#include "moter/pwmLAP.h"
#include "tcpLib/myClient/tcpClient.h"
#include "pastTime/pastTime.h"
#include "f9p/f9p_tcp.h"
#include "pointCalculation/vincenty.hpp"
#include "pointCalculation/cal_direction.h"
#include "hmc6352/hmc.h"

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include <iostream>
#include <string>

#include <queue>

#include <ulimit.h>
#include "logFile/myLog.h"

//using namespace std;
//------------------------------
// flg管理用関数
//------------------------------
//移動方向計算開始用flg
#define FLG_OPERATION_TCP_R 0
#define FLG_OPERATION_TIME_MOVE_RESULT 1
#define FLG_OPERATION_DISTINATION 2
#define FLG_OPERATION_DECISION_MAX 3

void allFlgFalse(bool flg[]);
bool ifAllFlgFalse(bool flg[]);

//------------------------------
// 移動先管理用関数
//------------------------------
bool flg_need_vincenty = false;

//------------------------------
// moter管理用関数
//------------------------------
#define MOTER_STRUCT msg3

#define DIR_PIN_R 13
#define PWM_PIN_R 6
#define DIR_PIN_L 12
#define PWM_PIN_L 5

#define M_OPERATION_S 0	 // スリープ
#define M_OPERATION_B 1	 // ブレーキ
#define M_OPERATION_MP 2 // 前進	//move progress
#define M_OPERATION_MB 3 // 後進	//move back
#define M_OPERATION_TR 4 // 右回転	//turn Right
#define M_OPERATION_TL 5 // 左回転	//turn left

#define ROTATION_REGULAR_MAX 255
#define ROTATION_INVERSE_MAX 0

//#define M_OPERATION_N 6	 // 未設定

MoterDriver10A moterL(PWM_PIN_L, DIR_PIN_L);
MoterDriver10A moterR(PWM_PIN_R, DIR_PIN_R);
void moterSetup();
int moter2operation(int mL, int mR);
void operation2moter(int mOp, MOTER_STRUCT *moter, int time_ms);

//------------------------------
// myServerTcp管理用関数
//------------------------------
int getSd_and_settingClient(char *hostnm, tcpStruct *client);
int getMyTcpFromServer2(int sd, tcpStruct *serverR);
int analaysisReadServerData(tcpStruct serverR, bool flgOD[], int *op, MOTER_STRUCT *moter, bool *flgTimer, int *wantTime, DISTINATION_STRUCT *LonLat);

myLogFile monitorLog;

int myMonitorPrint(int sd, tcpStruct *client, const char str[])
{
	client->flgMonitor = true;
	strcpy(client->buf, str);
	printf("%s", client->buf);
	monitorLog.print(str);
	if (clientSend(sd, client, sizeof(tcpStruct)) == 1) //データ送信実行
	{
		const char errStr[] = "err tcp Send\n";
		client->flgMonitor = false;
		printf("%s", errStr);
		//monitorLog.print(errStr);
		return -1;
	}
	client->flgMonitor = false;
	return 0;
}

void logStartMsg()
{
	printf("log:%s\n", monitorLog.filename.c_str());
	monitorLog.print("\n");
	monitorLog.print("\n");
	monitorLog.print("\n");
	monitorLog.print("------------------------------\n");
	monitorLog.print("rover start\n");
	monitorLog.print("------------------------------\n");
	monitorLog.print("\n");
}

//------------------------------
// f9p管理用関数
//------------------------------
int f9p_read_totalProcess(F9P_TCP *myF9P);
int flg_f9p_can = false;

//------------------------------
// log管理用関数
//------------------------------
bool flg_log_print = false;
int logNum = 0;
myLogFile nmeaLog(PATH_MODE__NMEA);

//------------------------------
// 計算モード管理用関数
//------------------------------
#define CALCULATION_MODE_NULL 0
#define CALCULATION_MODE_MOTER_RAW 1
#define CALCULATION_MODE_POINT_CALCULATION 2

int calculationMode = CALCULATION_MODE_NULL;

//------------------------------
// main関数
//------------------------------
int main(int argc, char *argv[])
{
	if (argc != 4 && argc != 2)
	{
		printf("option : myTcpServerAddress [f9p_adress f9p_Port]\n");
		return 0;
	}

	//time測定用変数
	bool flgTimeMove = false;
	int wantPastTime = 0;

	//moter setup
	int mOperation = 0, bMOpration = 0;
	MOTER_STRUCT nowMoter, bMoter;
	moterSetup();

	//f9p
	F9P_TCP myF9P;
	int f9p_stat;
	if (argc == 4)
	{
		flg_f9p_can = true;
		myF9P.access(argv[2], argv[3]);
		printf("connect read f9p[%s:%s]\n", argv[2], argv[3]);
		nmeaLog.printSimple("\n\n");
	}
	for (int i = 0; i < F9P_STR_ELEMENT; i++)
	{
		myF9P.sStr[i] = NULL;
	}

	//myTcpConnect
	tcpStruct clientS, mainServerR, tempServerR;
	int sd;
	sd = getSd_and_settingClient(argv[1], &clientS);
	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}
	clientS.flgMonitor = false;

	//キュー関係
	std::queue<tcpStruct> queueServer;
	bool flgQueuePop = true;
	bool flgQueuePopData_nowMove = false;

	//動作フラグ管理
	bool flgOperationDecision[FLG_OPERATION_DECISION_MAX];
	allFlgFalse(flgOperationDecision);

	// point calculation
	DISTINATION_STRUCT nowLonLat, tcpLonLat;
	my_lib::Vincenty v_cal(0, 0);
	//nowLonLatの初期化
	init_struct_distination(&nowLonLat);

	init_struct_distination(&tcpLonLat);

	//方位
	double nowDirection = 0;
	int tempDirection = 0;
	bool flgDirectionSenser = false;
	initHMC();

	logStartMsg();
	printf("start main\n\n");
	while (1)
	{
		//myTcpServerとのTCP受信.....ある程度最新版まで受け取っていることを確認したい
		switch (getMyTcpFromServer2(sd, &tempServerR))
		{
		case 0:
			break;
		case 1:
			if (tempServerR.queueAction == QUEUE__PUSH_AND_POP_ALLOWED)
				flgQueuePop = true;
			else if (tempServerR.queueAction == QUEUE__PUSH_AND_POP_DISALLOWED)
				flgQueuePop = false;
			else if (tempServerR.queueAction == QUEUE__DO_NOW_AND_STACK_REMOVE)
			{
				while (!queueServer.empty())
				{
					queueServer.pop();
				}
				flgQueuePop = true;
				flgQueuePopData_nowMove = false;
			}
			else
			{
				printf("err pop flg\n");
				continue;
			}
			queueServer.push(tempServerR);
			break;
		case -1:
		case -2:
			return 1;
		default:
			printf("err getMyTcpFromServer2()");
			break;
		}
		if ((flgQueuePop == true) && (flgQueuePopData_nowMove == false))
		{
			if (!queueServer.empty())
			{
				mainServerR = queueServer.front();
				queueServer.pop();
				analaysisReadServerData(mainServerR, flgOperationDecision, &mOperation, &nowMoter, &flgTimeMove, &wantPastTime, &tcpLonLat);
				flgQueuePopData_nowMove = true;
			}
		}

		//myTcpServerとのTCP送信(未実験)
		/*
		if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
			return -1;										  //err
		*/

		//f9pとのTCP
		if (flg_f9p_can)
		{
			f9p_stat = f9p_read_totalProcess(&myF9P);
			if (f9p_stat != 0)
			{
				switch (f9p_stat)
				{
				case 1:
					//get GGA
					//f9pの区切った結果のdata取得
					nowLonLat.now.lon = strtod(myF9P.sStr[GGA_LON], NULL) / 100.0;
					nowLonLat.now.lat = strtod(myF9P.sStr[GGA_LAT], NULL) / 100.0;
					nowLonLat.measurementResultType = strtol(myF9P.sStr[GGA_RTK], NULL, 10);
					//緯度(lat)・経度(lon)
					//nowLonLat.now.lat = 35.46810000;
					//nowLonLat.now.lon = 133.04860000;
					//printf("%lf, %lf, %d\n",nowLonLat.now.lon,nowLonLat.now.lat,nowLonLat.measurementResultType);
					break;
				case 2:
					//get RMC
					break;
				case 3:
					//get VTG
					break;
				default:
					break;
				}
			}
		}

		//センサー取得
		//方位
		tempDirection = getHMC();
		if (tempDirection != -1)
		{
			nowDirection = tempDirection;
			flgDirectionSenser = true;
		}
		else
		{
			flgDirectionSenser = false;
		}

		//時間系
		if ((flgTimeMove) && (getPastTime() > wantPastTime)) //規定時間を経過
		{
			flgOperationDecision[FLG_OPERATION_TIME_MOVE_RESULT] = true;
			flgTimeMove = false;
		}

		//動作方向決定に必要なフラグが集まった？
		//	前回と移動方向変化なら、各種データ初期化
		if (!ifAllFlgFalse(flgOperationDecision))
		{
			if (flgOperationDecision[FLG_OPERATION_TIME_MOVE_RESULT])
			{

				flgQueuePopData_nowMove = false;

				nowMoter.moterL = M_PWM_SLEEP;
				nowMoter.moterR = M_PWM_SLEEP;
				mOperation = M_OPERATION_S;
				calculationMode = CALCULATION_MODE_NULL;
				printf("stop by timer(%dms)\n", wantPastTime);
			}
			else if (flgOperationDecision[FLG_OPERATION_TCP_R])
			{
				calculationMode = CALCULATION_MODE_MOTER_RAW;
				printf("moter operation : %d\n", mOperation);
				//myサーバからTCPデータを受信
			}
			else if (flgOperationDecision[FLG_OPERATION_DISTINATION])
			{
				nowLonLat.distination = tcpLonLat.distination;
				nowLonLat.end = tcpLonLat.end;
				nowLonLat.sendType = tcpLonLat.sendType;

				calculationMode = CALCULATION_MODE_POINT_CALCULATION;
				flg_need_vincenty = true;
				printf("tcp get distination or LonLat\n");
			}
			else
			{
				printf("全てのフラグを消化していません\n");
			}
			allFlgFalse(flgOperationDecision);
			flg_log_print = true;
		}

		//計算
		//LON_LATの目標地点の設定
		if (flg_need_vincenty)
		{
			printf("v_cal start\n");

			std::tuple<double, double, double> t;
			nowLonLat.start = nowLonLat.now;
			v_cal.set(nowLonLat.start.lat, nowLonLat.start.lon);
			if (nowLonLat.sendType == RECORD_END_LON_LAT) //目標地点が記録されているので、目標方位と距離を計算
			{
				t = v_cal.calc_distance(nowLonLat.end.lat, nowLonLat.end.lon);
				nowLonLat.distination.km = std::get<0>(t);
				nowLonLat.distination.direction = std::get<1>(t);
			}
			else if (nowLonLat.sendType == RECORD_END_DISTINATION) //目標距離と方位が記録されているので、目標地点を計算
			{
				t = v_cal.calc_destination(nowLonLat.distination.direction, nowLonLat.distination.km);
				nowLonLat.end.lat = std::get<0>(t);
				nowLonLat.end.lon = std::get<1>(t);
			}
			else
			{
				printf("err set vincenty\n");
				flgQueuePopData_nowMove = false;
				if (nowLonLat.sendType == RECORD_RESULT)
				{
				}
			}
			nowLonLat.sendType = RECORD_RESULT;
			flg_need_vincenty = false;
		}

		//現在のLON_LATの計算
		if (calculationMode == CALCULATION_MODE_POINT_CALCULATION)
		{
			//目標地点が記録されているので、目標までの方位と残りの距離を計算
			v_cal.set(nowLonLat.now.lat, nowLonLat.now.lon);
			std::tuple<double, double, double> r;
			r = v_cal.calc_distance(nowLonLat.distination.direction, nowLonLat.distination.km);
			double range_to_target_km = std::get<0>(r);
			double range_to_target_Direction = std::get<1>(r);
			//目標地点まで来ているかの確認
			if (abs(range_to_target_km - nowLonLat.distination.km) < 0.001) //0.001km=1m
			{
				calculationMode = CALCULATION_MODE_NULL;
				nowMoter.moterL = M_PWM_SLEEP;
				nowMoter.moterR = M_PWM_SLEEP;
				mOperation = moter2operation(nowMoter.moterL, nowMoter.moterR);

				flgQueuePopData_nowMove = false;
				printf("LanLot目標地点に到着\n");
			}
			else
			{
				if (flgDirectionSenser == true)
				{
					double differenceDirectionRaw = differenceDirection(range_to_target_Direction, nowDirection);

					//p制御による移動方向の操作
					if (ifDirectionRange(range_to_target_Direction, nowDirection, 15.0))
					{
						mOperation = M_OPERATION_MP;
					}
					else
					{
						if (ifDirectionRange(range_to_target_Direction + 90, nowDirection, 90.0))
						{
							mOperation = M_OPERATION_TL;
						}
						else
						{
							mOperation = M_OPERATION_TR;
						}
					}
					operation2moter(M_OPERATION_S, &nowMoter, 100);
				}
				else
				{
					printf("err : Because there is no 'direction Senser', moter power for 'point calculation' cannot be calculated .\n");
					calculationMode = CALCULATION_MODE_NULL;
					nowMoter.moterL = M_PWM_SLEEP;
					nowMoter.moterR = M_PWM_SLEEP;
					mOperation = moter2operation(nowMoter.moterL, nowMoter.moterR);

					flgQueuePopData_nowMove = false;
				}
			}
		}
		else
		{
			//LON_LATに関する計算を行わない
			/*
			if (calculationMode == CALCULATION_MODE_MOTER_RAW){			}
			else if (calculationMode == CALCULATION_MODE_NULL){			}
			else{			}
			*/
		}

		bMOpration = mOperation;

		//モーター動作
		moterR.rotation(nowMoter.moterR);
		moterL.rotation(nowMoter.moterL);
		bMoter = nowMoter;

		//ログデータを取ることを考える
		char temp_str[256];
		if (flg_log_print)
		{

			myMonitorPrint(sd, &clientS, "----------\n");
			sprintf(temp_str, "log : %d\n", logNum);
			myMonitorPrint(sd, &clientS, temp_str);
			myMonitorPrint(sd, &clientS, "----------\n");

			sprintf(temp_str, "tcp getType %d\n", nowLonLat.sendType);
			myMonitorPrint(sd, &clientS, temp_str);

			//moter関係
			sprintf(temp_str, "(mL, mR)=(%3d, %3d), op=%d\n", nowMoter.moterL, nowMoter.moterR, mOperation);
			myMonitorPrint(sd, &clientS, temp_str);
			sprintf(temp_str, "wantPastTime=(%5d)\n", wantPastTime);
			myMonitorPrint(sd, &clientS, temp_str);

			//座標関係
			printf("緯度(lat)・経度(lon)\n");
			sprintf(temp_str, "n(lat,lon)=(%lf, %lf),\n", nowLonLat.now.lat, nowLonLat.now.lon);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "s(lat,lon)=(%lf, %lf),\n", nowLonLat.start.lat, nowLonLat.start.lon);
			myMonitorPrint(sd, &clientS, temp_str);
			sprintf(temp_str, "e(lat,lon)=(%lf, %lf), \n", nowLonLat.end.lat, nowLonLat.end.lon);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "(km, direction)=(%lf, %lf)\n", nowLonLat.distination.km, nowLonLat.distination.direction);
			myMonitorPrint(sd, &clientS, temp_str);

			//f9p関係
			sprintf(temp_str, "f9p connect : %s\n", (flg_f9p_can ? "true" : "false"));
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "rtkResult : %d\n", nowLonLat.measurementResultType);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "hmc connect : %s\n", (flgDirectionSenser ? "true" : "false"));
			myMonitorPrint(sd, &clientS, temp_str);

			myMonitorPrint(sd, &clientS, "----------\n");

			myMonitorPrint(sd, &clientS, "\n");

			flg_log_print = false;
			logNum++;
		}
		if (kbhit())
		{
			printf("\nキー入力を検知\n");
			return 0;
		}
	}
}

//------------------------------
// flg管理用関数
//------------------------------
//動作判定開始用関数の初期化
void allFlgFalse(bool flg[])
{
	for (int i = 0; i < FLG_OPERATION_DECISION_MAX; i++)
		flg[i] = false;
}

//動作判定用関数の確認
bool ifAllFlgFalse(bool flg[])
{
	for (int i = 0; i < FLG_OPERATION_DECISION_MAX; i++)
	{
		if (flg[i])
			return false;
	}
	return true;
}

//------------------------------
// moter管理用関数
//------------------------------
//モータの初期設定(停止)
void moterSetup()
{
	//setRotationDirectionで二つのモータに＋値を入力時に前進
	moterL.setRotationDirection(0);
	moterR.setRotationDirection(0);
	moterL.stop();
	moterR.stop();
}

//　目的:モータの設定から動作の種類を確認
int moter2operation(int mL, int mR)
{
	if ((mL == M_PWM_STOP) && (mR == M_PWM_STOP))
	{
		return M_OPERATION_B;
	}
	else if ((mL == M_PWM_SLEEP) || (mR == M_PWM_SLEEP))
	{
		return M_OPERATION_S;
	}
	else
	{
		if (mL <= M_PWM_STOP)
		{
			if (mR <= M_PWM_STOP)
				return M_OPERATION_MP;
			else
				return M_OPERATION_TR;
		}
		else
		{
			if (mR <= M_PWM_STOP)
				return M_OPERATION_TL;
			else
				return M_OPERATION_MB;
		}
	}
	printf("モータの設定値がおかしいです\n");
	return M_OPERATION_S;
}

void operation2moter(int mOp, MOTER_STRUCT *moter, int time_ms)
{
	moter->time = time_ms;
	switch (mOp)
	{
	case M_OPERATION_S:
		moter->moterL = M_PWM_SLEEP;
		moter->moterR = M_PWM_SLEEP;
		break;
	case M_OPERATION_B:
		moter->moterL = M_PWM_STOP;
		moter->moterR = M_PWM_STOP;
		break;
	case M_OPERATION_MP:
		moter->moterL = ROTATION_REGULAR_MAX;
		moter->moterR = ROTATION_REGULAR_MAX;
		break;
	case M_OPERATION_MB:
		moter->moterL = ROTATION_INVERSE_MAX;
		moter->moterR = ROTATION_INVERSE_MAX;
		break;
	case M_OPERATION_TR:
		moter->moterL = ROTATION_REGULAR_MAX;
		moter->moterR = ROTATION_INVERSE_MAX;
		break;
	case M_OPERATION_TL:
		moter->moterL = ROTATION_INVERSE_MAX;
		moter->moterR = ROTATION_REGULAR_MAX;
		break;
	default:
		printf("err : operation2moter\n");
		break;
	}
}

//------------------------------
// myServerTcp管理用関数
//------------------------------
int getSd_and_settingClient(char *hostnm, tcpStruct *client)
{
	int sd;
	init_tcpStruct(client, ROVER_NAME);
	//int j = 0;
	client->dataType = 3; //送信するデータタイプ
	sd = myTcpConnect(hostnm, MY_TCP_POT);
	return sd;
}

//TCPサーバからデータを受信して、引数の変数に格納する
//戻り値:0=受信なし、1=受信成功、その他=通信エラー
int getMyTcpFromServer2(int sd, tcpStruct *serverR)
{
	tcpStruct readData;
	switch (clientRecv(sd, &readData, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
	{
	case 1:
		*serverR = readData;
		return 1;
		break; //読み込み成功
	case 0:
		return 0;
		break; //状態変化なし。
	case -1:
	case -2:
		printf("Err : tcpClient(1)\n");
		return -1; //err
	default:
		printf("Err : tcpClient(2)\n");
		return -2; //err
	}
	return 0;
}

int analaysisReadServerData(tcpStruct serverR, bool flgOD[], int *op, MOTER_STRUCT *moter, bool *flgTimer, int *wantTime, DISTINATION_STRUCT *LonLat)
{
	bool flgTimerMove = *flgTimer;
	char printBuf[50];
	if (serverR.dataType == 3)
	{
		int wantT = 0;

		//モータ速度変更用データ受信
		flgOD[FLG_OPERATION_TCP_R] = true;

		if (serverR.useData.data3.time > 0)
		{
			flgTimerMove = true;
			setBaselineTime();
			wantT = serverR.useData.data3.time;
		}

		moter->moterL = serverR.useData.data3.moterL;
		moter->moterR = serverR.useData.data3.moterR;
		*flgTimer = flgTimerMove;
		*wantTime = wantT;
		*op = moter2operation(moter->moterL, moter->moterR);
	}
	else if (serverR.dataType == 4)
	{
		flgOD[FLG_OPERATION_DISTINATION] = true;
		*LonLat = serverR.useData.data4;
		*wantTime = 0;
		*flgTimer = false;
	}
	else if (serverR.dataType == 5)
	{
		int wantT = 0, mL, mR;
		//モータ速度変更用データ受信
		flgOD[FLG_OPERATION_TCP_R] = true;

		if (serverR.useData.data5.time > 0)
		{
			flgTimerMove = true;
			setBaselineTime();
			wantT = serverR.useData.data5.time;
		}
		*flgTimer = flgTimerMove;
		*wantTime = wantT;
		*op = serverR.useData.data5.moterOperation;
		operation2moter(*op, moter, wantT);
	}
	else
	{
		printf("解析したデータは必要無いものでした。(dataType=%d) msg=%s\n", serverR.dataType, serverR.buf);
		//printf("received:from:%s, %s, msg:%s\n", serverR.userName, printBuf, serverR.buf);
	}
	return 0;
}

//------------------------------
// f9p管理用関数
//------------------------------
int f9p_read_totalProcess(F9P_TCP *myF9P)
{
	int stat;
	myF9P->scanRead();
	stat = myF9P->checkNmeaType();
	if (stat != 0)
	{
		myF9P->myStrtokS();
		nmeaLog.printSimple(myF9P->readBuffer);
		nmeaLog.printSimple("\n");
	}
	return stat;
}
