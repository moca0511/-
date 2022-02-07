#include <string.h>
#include "moter/roverMoter.h"
#include "pico_quick/pico_quick.h"
#include "logFile/myLog.h"
#include "pastTime/pastTime.h"
//#include "hmc/hmc.h"

#include "tcpServerClient/tcpClient.h"
#include "f9p/f9p_tcp.h"
#include "guide/guide.h"
#include "myQueue/myQueue.h"
#include "pointCalculation/vincenty.h"

//using namespace std;
//------------------------------
// flg管理用関数
//------------------------------
//移動方向計算開始用flg
#define FLG_OPERATION_TCP_R 0
#define FLG_OPERATION_TIME_MOVE_RESULT 1
#define FLG_OPERATION_DISTINATION 2
#define FLG_OPERATION_POINT_OFFSET 3
#define FLG_OPERATION_DECISION_MAX 4

void allFlgFalse(bool flg[]);
bool ifAllFlgFalse(bool flg[]);
bool ifArg(const int argc, char *argv[], const int num, const char checkStr[]);
bool flgPicoPrint_Before = false;
//------------------------------
//time測定用変数
//------------------------------

//------------------------------
// log管理用関数
//------------------------------

//------------------------------
// myServerTcp管理用関数
//------------------------------
#define USER_NAME "rover"
int getMyTcpFromServer2(int sd, tcpStruct *serverR);
int analaysisReadServerData(tcpStruct serverR, bool flgOD[], int *op, MOTER_STRUCT *moter, bool *flgTimer, int *wantTime, DISTINATION_STRUCT *LonLat);

// tcpClient*myQueue
int getMyTcpFromServer2_setQueue(int sd, MY_QUEUE *myQueue, bool *flgQ_popData);

// logFile * tcpClient
void logStartMsg(const char name[]);
int myMonitorPrint(int sd, tcpStruct *client, const char str[]);
int myMonitorPrintSimple(int sd, tcpStruct *client, const char str[]);

int sendTransfer_R2S(int sd, tcpStruct *client, LON_LAT nowLonLat, const double dir);
//------------------------------
// f9p管理用関数
//------------------------------
// f9p * logFile
int f9p_read_totalProcess(F9P_TCP *myF9P);
void getNowF9P(F9P_TCP *myF9P, DISTINATION_STRUCT *nowData, int *rtkResult);

//------------------------------
// 計算モード管理用関数
//------------------------------
#define CALCULATION_MODE_NULL 0
#define CALCULATION_MODE_MOTER_RAW 1
#define CALCULATION_MODE_POINT_CALCULATION 2

// moter * call_direction
int cal_moterPower(const double now, const double hope, double distance, MOTER_STRUCT *m);

//------------------------------
// rover main
//------------------------------
int main_rover(const int argc, char *argv[]);

//------------------------------
//time測定用変数
//------------------------------
myPastTime mainPastTime;
//myPastTime picoPastTime;

//------------------------------
// log管理用関数
//------------------------------
myLogFile monitorLog(false);
const bool flg_logMonitor_printf = true; //logMonitorへの出力を./rover.outでも出力するならture

//------------------------------
// f9p管理用関数
//------------------------------
// f9p * logFile
// ログの保存先を確認したいときはtrueにすることで、初回書き込み時にファイルの場所を出力する。
bool flg_f9pLog_NMEA_firstPrint = false; // nmeaデータの保存先
bool flg_f9pLogFile_firstPrint = false;	 // nmeaから抽出した緯度経度、rtkの結果の保存先

DISTINATION offsetDist;

//------------------------------
//led
//------------------------------
bool flgWriteLED = true;
myGpio writePin(19, GPIO_MODE_OUT);

//------------------------------
//icm
//------------------------------
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <ulimit.h>
#include <stdlib.h>
#include <unistd.h>
#define FIFO "/tmp/icm.fifo"

class myFIFO_ICM
{
private:
	int fd;
	char buf[256];
	int len;

public:
	myFIFO_ICM();
	~myFIFO_ICM();
	int myRead(double *rData);
};

//------------------------------
// main関数
//------------------------------
int main(int argc, char *argv[])
{
	main_rover(argc, argv);
}

//------------------------------
// rover main
//------------------------------
int main_rover(const int argc, char *argv[])
{
	// *** エラーが起きてクライアントが終了したことをサーバ側に知らせる為に、宣言の順番TCPサーバとの接続を初めに行う。
	//myTcpConnect
	tcpStruct clientS, mainServerR, tempServerR;
	int sd;
	sd = myTcpConnect(ADDR__MY_TCP_SERVER, MY_TCP_POT);
	if (sd <= -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}
	init_tcpStruct(&clientS, USER_NAME);
	clientS.dataType = 3; //送信するデータタイプ

	if ((argc < 1) || (argc > 6) || (argc == 5))
	{
		printf("option(%d) : [f9p-off [hmc-off[pico[direction m]]]]\n", argc);
		return 0;
	}

	//f9p
	F9P_TCP myF9P;
	int f9p_stat, flg_f9p_can = false;
	int flg_rtkFist = 0; //0=何も表示していない。1=rkt未成功と表示。2=rtk成功と表示
	double differenceDirectionRaw = 0;
	if (!ifArg(argc, argv, 2, "off"))
	{
		flg_f9p_can = true;
		myF9P.access(ADDR__F9P_RTK, TCP_PORT__F9P_RTK);
		printf("connect read f9p[%s:%s]\n", ADDR__F9P_RTK, TCP_PORT__F9P_RTK);
	}

	//方位
	myFIFO_ICM icm;
	double nowDirection = -1, tempDirection = 0;
	bool flgDirectionSenser = false;
	/*
	initHMC(HMC_DEFAULT_VALUE__ARGUMENT, 270);
	if (!ifArg(argc, argv, 3, "off"))
	{
		if (getHMC() == -1)
		{
			printf("hmc 接続エラー\n");
			return 0;
		}
	}
	*/

	//pico_i2c
	pico_quick myPico;
	// pico i2c管理用関数
	int flg_pico_can = false;
	myPico.getPico();

	if (!ifArg(argc, argv, 4, "off"))
	{
		flg_pico_can = true;
	}

	if (argc == 6)
	{
		offsetDist.direction = std::strtod(argv[4], NULL);
		offsetDist.m = std::strtod(argv[5], NULL);
		printf("offset dir=%lf, m=%f\n", offsetDist.direction, offsetDist.m);
	}

	//moter setup
	roverMoter moter;
	int mOperation = 0, bMOpration = 0, picoTmpOp = 0;
	MOTER_STRUCT nowMoter, bMoter, picoTmpM;
	moter.moterSetup();
	nowMoter.moterL = M_PWM_SLEEP;
	nowMoter.moterR = M_PWM_SLEEP;
	nowMoter.time = 0;

	//time測定用変数
	bool flgTimeMove = false;
	int wantPastTime = 0;
	myPastTime guidePastTIme;

	//キュー関係
	MY_QUEUE myQueue;
	bool flgQueuePopData_nowMove = false;

	//動作フラグ管理
	bool flgOperationDecision[FLG_OPERATION_DECISION_MAX];
	allFlgFalse(flgOperationDecision);

	// point calculation
	LON_LAT nowLonLat;
	MyGuide guide;
	init_lon_lat(&nowLonLat);
	bool flg_need_vincenty = false;
	int calculationMode = CALCULATION_MODE_NULL;

	//log関係
	bool flg_log_print = false;
	int logNum = 0;
	char temp_str[256];
	logStartMsg(USER_NAME);
	printf("tcpStruct size=%d\n", sizeof(tcpStruct));
	printf("rover main setup end\n\n");
	printf("rover main start\n\n");

	while (1)
	{
		//myTcpServerとのTCP受信.....ある程度最新版まで受け取っていることを確認したい
		getMyTcpFromServer2_setQueue(sd, &myQueue, &flgQueuePopData_nowMove);
		//ここでfifoで読み込みキューにプッシュする
		if ((myQueue.ifPop() == true) && (flgQueuePopData_nowMove == false))
		{
			mainServerR = myQueue.myPop();
			analaysisReadServerData(mainServerR, flgOperationDecision, &mOperation, &nowMoter, &flgTimeMove, &wantPastTime, &nowLonLat.target);
			flgQueuePopData_nowMove = true;
		}

		//myTcpServerとのTCP送信(未実験)
		/*
			if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
				return -1;										  //err
		*/

		//f9pとのTCP
		if (flg_f9p_can)
		{
			getNowF9P(&myF9P, &nowLonLat.now, &nowLonLat.rtkResult);
			if (flgOperationDecision[FLG_OPERATION_POINT_OFFSET])
			{
				//offsetLonLat.lat = mainServerR.useData.data6.offsetSupport.lat - nowLonLat.now.point.lat;
				//offsetLonLat.lon = mainServerR.useData.data6.offsetSupport.lon - nowLonLat.now.point.lon;
			}

			if (flg_rtkFist == 0)
			{
				if (nowLonLat.rtkResult == RTK_RESULT__FIX)
				{
					printf("RTK_FIX\n");
					flg_rtkFist = 2;
				}
				else
				{
					printf("NOT_RTK_FIX\n");
					flg_rtkFist = 2;
				}
			}
			else if (flg_rtkFist == 1)
			{
				if (nowLonLat.rtkResult == RTK_RESULT__FIX)
				{
					printf("RTK_FIX\n");
					flg_rtkFist = 2;
				}
			}
		}

		//センサー取得
		//方位
		if (icm.myRead(&tempDirection) == 1)
		{
			if ((tempDirection == 0.0) || (tempDirection == -1) || (tempDirection == 360.0))
			{
				//flgDirectionSenser = false;
			}
			else
			{
				flgDirectionSenser = true;
				nowDirection = tempDirection;
			}
		}

		/*
		flgDirectionSenser = false;
		tempDirection = getHMC();
		if (tempDirection != -1)
		{
			nowDirection = tempDirection;
			flgDirectionSenser = true;
		}
		*/
		//時間系
		if ((flgTimeMove) && (mainPastTime.getPastTime() > wantPastTime)) //規定時間を経過
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
				calculationMode = CALCULATION_MODE_POINT_CALCULATION;
				flg_need_vincenty = true;
				printf("tcp get distination or LonLat %d\n", calculationMode);
			}
			else if (flgOperationDecision[FLG_OPERATION_POINT_OFFSET])
			{
				printf("rtk offset 計算実行フラグ\n");
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
			switch (nowLonLat.rtkResult)
			{
			case RTK_RESULT__FLOT:
				printf("rtk_result=Flot\n");
			case RTK_RESULT__FIX:
				guide.setGuideTarget(nowDirection, nowLonLat);
				nowLonLat = guide.get();

				printf("v_cal start(lat,lon)=(%f, %f)\n", guide.get().start.lat, guide.get().start.lon);
				break;
			default:
				printf("rtk_result!=(Flot or Fix)\n");
				calculationMode == CALCULATION_MODE_NULL;
				break;
			}
			flg_need_vincenty = false;
		}

		//現在のLON_LATの計算
		//目標地点が記録されているので、目標までの方位と残りの距離を計算
		//目標地点まで来ているかの確認
		if ((calculationMode == CALCULATION_MODE_POINT_CALCULATION) && (flgDirectionSenser == true) && (nowLonLat.now.recodType == RECORD_END_LON_LAT))
		{
			std::tuple<int, double> r;
			int stauts_callNow;
			switch (nowLonLat.rtkResult)
			{
			case RTK_RESULT__FLOT:
				printf("rtk_result=Flot\n");
			case RTK_RESULT__FIX:
				//printf("現在の座標計算開始\n");
				r = guide.calNow(nowDirection, nowLonLat);
				stauts_callNow = std::get<0>(r);
				if (stauts_callNow == 0)
				{
					//printf("現在の座標計算成功\n");
					nowLonLat = guide.get();
					differenceDirectionRaw = std::get<1>(r);
					if (abs(nowLonLat.now.distination.m) < 0.2) //0.001km=1m
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
						//p制御による移動方向の操作
						mOperation = cal_moterPower(nowDirection, nowLonLat.now.distination.direction, abs(nowLonLat.now.distination.m), &nowMoter);
						//mOperation = cal_moterPower_feedback(nowDirection,nowLonLat.now.distination.direction,  &nowMoter, 15);
					}
				}
				else
				{
					//ここまでに方位センサとf9Pには適切に接続て来ているのにここに来る場合おかしい。
					printf("false guide.call_Now\n");
					if (stauts_callNow == -2)
						printf("方位センサーが未接続\n");
					else if (stauts_callNow == -1)
						printf("現在位置情報にエラー\n");
					exit(1);
				}
				break;
			default:
				printf("rtk_result!=(Flot or Fix) なので、モータ停止\n");
				nowMoter.moterL = M_PWM_SLEEP;
				nowMoter.moterR = M_PWM_SLEEP;
				mOperation = moter2operation(nowMoter.moterL, nowMoter.moterR);
				break;
			}
		}
		else
		{
			if ((calculationMode == CALCULATION_MODE_POINT_CALCULATION) && (nowLonLat.now.recodType != RECORD_RESULT))
			{
				printf("err : 方位センサー未接続か、現在座標が取得できていないため、現在の移動するべき方向が計算できません, %d, %d\n", flgDirectionSenser, nowLonLat.now.recodType);
				calculationMode = CALCULATION_MODE_NULL;
				nowMoter.moterL = M_PWM_SLEEP;
				nowMoter.moterR = M_PWM_SLEEP;
				mOperation = moter2operation(nowMoter.moterL, nowMoter.moterR);
				flgQueuePopData_nowMove = false;
				flg_log_print = false;
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
		}

		if (flg_pico_can == true)
		{
			myPico.getPico();
			/*if (picoPastTime.getPastTime() > 5)
			{
				//myPico.getPico();
				picoPastTime.setBaselineTime();
			}*/

			if (myPico.getWallFlg(WALL_FRONT) == 1)
			{
				if (mOperation == M_OPERATION_MP)
				{
					if (flgPicoPrint_Before == false)
					{
						picoTmpOp = mOperation;
						picoTmpM = nowMoter;
						printf("pico_on\n");
						flgPicoPrint_Before = true;
					}
					nowMoter.moterL = 128;
					nowMoter.moterR = 128;
				}
				else
				{
					if (flgPicoPrint_Before == true)
					{
						printf("pico_off\n");
						flgPicoPrint_Before = false;
						//if (picoTmpOp == mOperation)
						//nowMoter = picoTmpM;
					}
				}
			}

			if (myPico.getWallFlg(WALL_POST) == 1)
			{
				if (mOperation == M_OPERATION_MB)
				{
					if (flgPicoPrint_Before == false)
					{
						picoTmpOp = mOperation;
						picoTmpM = nowMoter;
						printf("pico_on\n");
						flgPicoPrint_Before = true;
					}
					nowMoter.moterL = 128;
					nowMoter.moterR = 128;
				}
				else
				{
					if (flgPicoPrint_Before == true)
					{
						printf("pico_off\n");
						flgPicoPrint_Before = false;
						//if (picoTmpOp == mOperation)
						//nowMoter = picoTmpM;
					}
				}
			}

			if ((myPico.getWallFlg(WALL_LEFT) == 1) || (myPico.getWallFlg(WALL_RIGHT) == 1))
			{
				if ((mOperation == M_OPERATION_TL) || (mOperation == M_OPERATION_TR))
				{
					if (flgPicoPrint_Before == false)
					{
						picoTmpOp = mOperation;
						picoTmpM = nowMoter;
						printf("pico_on\n");
						flgPicoPrint_Before = true;
					}
					nowMoter.moterL = 128;
					nowMoter.moterR = 128;
				}
				else
				{
					if (flgPicoPrint_Before == true)
					{
						printf("pico_off\n");
						flgPicoPrint_Before = false;
						//if (picoTmpOp == mOperation)
						//nowMoter = picoTmpM;
					}
				}
			}
		}

		//モーター動作
		moter.R.rotation(nowMoter.moterR);
		moter.L.rotation(nowMoter.moterL);
		bMoter = nowMoter;
		bMOpration = mOperation;
		/*
		//ログデータを取ることを考える
		if (flg_log_print)
		{
			myMonitorPrint(sd, &clientS, "----------\n");
			sprintf(temp_str, "log : %d\n", logNum);
			myMonitorPrint(sd, &clientS, temp_str);
			myMonitorPrint(sd, &clientS, "----------\n");

			sprintf(temp_str, "tcp getType %d\n", nowLonLat.target.recodType);
			myMonitorPrint(sd, &clientS, temp_str);

			//moter関係
			sprintf(temp_str, "(mL, mR)=(%3d, %3d), op=%d\n", nowMoter.moterL, nowMoter.moterR, mOperation);
			myMonitorPrint(sd, &clientS, temp_str);
			sprintf(temp_str, "wantPastTime=(%5d)\n", wantPastTime);
			myMonitorPrint(sd, &clientS, temp_str);

			//座標関係
			myMonitorPrint(sd, &clientS, "緯度(lat)・経度(lon)\n");

			sprintf(temp_str, "n(lat,lon)=(%lf, %lf),\n", nowLonLat.now.point.lat, nowLonLat.now.point.lon);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "s(lat,lon)=(%lf, %lf),\n", nowLonLat.start.lat, nowLonLat.start.lon);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "e(lat,lon)=(%lf, %lf), \n", nowLonLat.target.point.lat, nowLonLat.target.point.lon);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "s->e(m, direction)=(%lf, %lf)\n", nowLonLat.target.distination.m, nowLonLat.target.distination.direction);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "n->e(m, direction)=(%lf, %lf)\n", nowLonLat.now.distination.m, nowLonLat.now.distination.direction);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "differenceDirectionRaw=%f\n", differenceDirectionRaw);
			myMonitorPrint(sd, &clientS, temp_str);

			//f9p関係
			sprintf(temp_str, "f9p connect : %s\n", (flg_f9p_can ? "true" : "false"));
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "rtkResult : %d\n", nowLonLat.rtkResult);
			myMonitorPrint(sd, &clientS, temp_str);

			sprintf(temp_str, "hmc connect : %s\n", (flgDirectionSenser ? "true" : "false"));
			myMonitorPrint(sd, &clientS, temp_str);

			myMonitorPrint(sd, &clientS, "----------\n");

			myMonitorPrint(sd, &clientS, "\n");

			flg_log_print = false;
			logNum++;
		}
*/
		if (guidePastTIme.getPastTime() > 1000)
		{
			if (flgWriteLED == false)
				flgWriteLED = true;
			else
				flgWriteLED = false;
			writePin.myWrite(flgWriteLED);
			//printf("%lf\n", nowDirection);
			sendTransfer_R2S(sd, &clientS, nowLonLat, nowDirection);

			sprintf(temp_str, "f9p:%s(%d), ", ((nowLonLat.rtkResult == RTK_RESULT__FIX) ? "fix" : "no_fix"), nowLonLat.rtkResult);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "nowDir=%3.1lf, ", nowDirection);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "n->e(m)=%7.2f, ", (nowLonLat.now.distination.m));
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "diffDir=%4.0f, ", differenceDirectionRaw);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "cal=%d, ", calculationMode);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "mOp=%d, L=%3d, R=%3d, ", mOperation, nowMoter.moterL, nowMoter.moterR);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			myMonitorPrintSimple(sd, &clientS, "(lat,lon), ");

			sprintf(temp_str, "s(%.10lf, %.10lf), ", nowLonLat.start.lat, nowLonLat.start.lon);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "e(%.10lf, %.10lf), ", nowLonLat.target.point.lat, nowLonLat.target.point.lon);
			myMonitorPrintSimple(sd, &clientS, temp_str);

			sprintf(temp_str, "n(%.10lf, %.10lf), ", nowLonLat.now.point.lat, nowLonLat.now.point.lon);
			myMonitorPrintSimple(sd, &clientS, temp_str);
			sprintf(temp_str, "wall:front=%d, right=%d, post=%d, left=%d, ", myPico.getWallFlg(WALL_FRONT), myPico.getWallFlg(WALL_RIGHT), myPico.getWallFlg(WALL_POST), myPico.getWallFlg(WALL_LEFT));
			myMonitorPrintSimple(sd, &clientS, temp_str);

			myMonitorPrintSimple(sd, &clientS, "\n");
			fflush(stdout);
			guidePastTIme.setBaselineTime();
		}

		if (kbhit())
		{
			printf("\nキー入力を検知\n");
			return 0;
		}
		//usleep(100 * 1);
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

bool ifArg(const int argc, char *argv[], const int num, const char checkStr[])
{
	if ((argc >= num) && (strcmp(argv[num - 1], checkStr) == 0))
		return true;
	return false;
}

//------------------------------
// myServerTcp管理用関数
//------------------------------
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

int getMyTcpFromServer2_setQueue(int sd, MY_QUEUE *myQueue, bool *flgQ_popData)
{
	tcpStruct tempServerR;
	switch (getMyTcpFromServer2(sd, &tempServerR))
	{
	case 0:
		break;
	case 1:
		*flgQ_popData = myQueue->setPush(tempServerR, *flgQ_popData);
		break;
	case -1:
	case -2:
		exit(1);
	default:
		printf("err getMyTcpFromServer2()");
		exit(1);
	}
	return 0;
}

int analaysisReadServerData(tcpStruct serverR, bool flgOD[], int *op, MOTER_STRUCT *moter, bool *flgTimer, int *wantTime, DISTINATION_STRUCT *LonLat)
{
	bool flgTimerMove = *flgTimer;
	char printBuf[50];
	if (serverR.dataType == 3) //モータ速度変更用データ受信
	{
		int wantT = 0;
		flgOD[FLG_OPERATION_TCP_R] = true;
		if (serverR.useData.data3.time > 0)
		{
			flgTimerMove = true;
			mainPastTime.setBaselineTime();
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
	else if (serverR.dataType == 5) //モータ速度変更用データ受信
	{
		int wantT = 0, mL, mR;
		flgOD[FLG_OPERATION_TCP_R] = true;
		if (serverR.useData.data5.time > 0)
		{
			flgTimerMove = true;
			mainPastTime.setBaselineTime();
			wantT = serverR.useData.data5.time;
		}
		*flgTimer = flgTimerMove;
		*wantTime = wantT;
		*op = serverR.useData.data5.moterOperation;
		operation2moter(*op, moter, wantT);
	}
	else if (serverR.dataType == 6) //RTK結果に対するオフセット計算
	{
		flgOD[FLG_OPERATION_POINT_OFFSET] = true;
		//offset用データは一度しか利用しないので、個別変数に移動する必要がない
	}
	else
	{
		printf("解析したデータは必要無いものでした。(dataType=%d) msg=%s\n", serverR.dataType, serverR.buf);
		//printf("received:from:%s, %s, msg:%s\n", serverR.userName, printBuf, serverR.buf);
	}
	return 0;
}

int myMonitorPrint(int sd, tcpStruct *client, const char str[])
{
	client->flgMonitor = true;
	strcpy(client->buf, str);
	if (flg_logMonitor_printf)
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

int myMonitorPrintSimple(int sd, tcpStruct *client, const char str[])
{
	client->flgMonitor = true;
	strcpy(client->buf, str);
	if (flg_logMonitor_printf)
		printf("%s", client->buf);
	monitorLog.printSimple(str);
	if (clientSend(sd, client, sizeof(tcpStruct)) == 1) //データ送信実行
	{
		const char errStr[] = "err tcp Send\n";
		client->flgMonitor = false;
		printf("%s", errStr);
		monitorLog.print(errStr);
		return -1;
	}
	client->flgMonitor = false;
	return 0;
}

void logStartMsg(const char name[])
{
	char nameTemp[200];
	char tmp[300];
	sprintf(nameTemp, "%s start\n", name);
	printf("main monitor log:%s\n", monitorLog.filename.c_str());
	sprintf(tmp, "\n\n\n------------------------------\n%s------------------------------\n\n", nameTemp);
	printf("logStartTmp \n");
	sleep(1);
	monitorLog.print(tmp);
	sleep(1);
	printf("first log end\n");
}

//------------------------------
// f9p管理用関数
//------------------------------
int f9p_read_totalProcess(F9P_TCP *myF9P)
{
	int stat;
	myLogFile nmeaLog(PATH_MODE__NMEA, flg_f9pLog_NMEA_firstPrint);
	if (flg_f9pLog_NMEA_firstPrint)
	{
		nmeaLog.printSimple("\n\n");
		flg_f9pLog_NMEA_firstPrint = false;
	}
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

void getNowF9P(F9P_TCP *myF9P, DISTINATION_STRUCT *nowData, int *rtkResult)
{
	const char logCsvPath[] = "./log/lon_lat_now.csv";
	int f9p_stat;
	std::tuple<double, double, double> ttt;
	my_lib::Vincenty tmpV(0, 0);
	myLogFile log_csv_nowLonLat(logCsvPath, flg_f9pLogFile_firstPrint);
	if (flg_f9pLogFile_firstPrint)
	{
		log_csv_nowLonLat.printSimple("lat, lon, lat_raw, lon_raw, rtk\n");
		flg_f9pLogFile_firstPrint = false;
	}
	else
	{
		if ((f9p_stat = f9p_read_totalProcess(myF9P)) != 0)
		{
			switch (f9p_stat)
			{
			case 1:
				//get GGA
				//f9pの区切った結果のdata取得
				//緯度(lat)・経度(lon)
				char temp[500];
				nowData->point.lon = myF9P->convStr_nmea2google(GGA_LON);
				nowData->point.lat = myF9P->convStr_nmea2google(GGA_LAT);
				nowData->recodType = RECORD_END_LON_LAT;
				*rtkResult = strtol(myF9P->sStr[GGA_RTK], NULL, 10);

				tmpV.set(nowData->point.lat, nowData->point.lon);
				ttt = tmpV.calc_destination(offsetDist.direction, offsetDist.m);
				nowData->point.lat = std::get<0>(ttt);
				nowData->point.lon = std::get<1>(ttt);

				sprintf(temp, "%.15f, %.15f ", nowData->point.lat, nowData->point.lon);
				log_csv_nowLonLat.printSimple(temp);

				sprintf(temp, "%s, %s, ", myF9P->sStr[GGA_LAT], myF9P->sStr[GGA_LON]);
				log_csv_nowLonLat.printSimple(temp);

				sprintf(temp, "%d ", *rtkResult);
				log_csv_nowLonLat.printSimple(temp);

				log_csv_nowLonLat.printSimple("\n");
				//lon_lat.target.now.lat = 35.46810000;
				//lon_lat.target.now.lon = 133.04860000;
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
}

//------------------------------

int cal_moterPower(const double now, const double hope, double distance, MOTER_STRUCT *m)
{
	int mOperation, power = POWER_MAX;
	if (ifDirectionRange(hope, now, 30.0))
	{
		mOperation = M_OPERATION_MP;
		if (ifDirectionRange(hope, now, 5.0))
		{
			if (distance < 1.0)
				power = POWER_HALF;
			mOperation = M_OPERATION_MP;
		}
		else
		{
			m->time = 100;
			if (ifDirectionRange(hope + 90, now, 90.0))
			{
				m->moterL = ROTATION_REGULAR_LOW;
				m->moterR = ROTATION_REGULAR_MAX;
			}
			else
			{
				m->moterL = ROTATION_REGULAR_MAX;
				m->moterR = ROTATION_REGULAR_LOW;
			}
			return mOperation;
		}
	}
	else
	{
		if (ifDirectionRange(hope + 180, now, 90.0) && (distance < 0.5))
		{
			//近くで、後側に目標地点がある
			power = POWER_LOW;
			mOperation = M_OPERATION_MB;
		}
		else
		{
			if (ifDirectionRange(hope + 90, now, 90.0))
				mOperation = M_OPERATION_TL;
			else
				mOperation = M_OPERATION_TR;
			if (ifDirectionRange(hope, now, 30.0))
				power = POWER_LOW;
			if (ifDirectionRange(hope, now, 90.0))
				power = POWER_HALF;
		}
	}
	operation2moter(mOperation, m, 100, power);
	return mOperation;
}

int sendTransfer_R2S(int sd, tcpStruct *client, LON_LAT nowLonLat, const double dir)
{
	client->flgTransfer_R2S = true;

	int a, b, c;
	char tmp[1024];
	sprintf(tmp, "{ \"mode\": %d, \"lat\": %20.15lf, \"lng\": %20.15lf, \"dir\": %6.2lf }\n\0",
			nowLonLat.rtkResult, nowLonLat.now.point.lat, nowLonLat.now.point.lon, dir);
	//			client->useData.data4.recodType, client->useData.data4.point.lat, client->useData.data4.point.lon);

	//printf("%s\n", tmp);

	strcpy(client->buf, tmp);

	if (clientSend(sd, client, sizeof(tcpStruct)) == 1) //データ送信実行
	{
		const char errStr[] = "err tcp Send\n";
		client->flgTransfer_R2S = false;
		printf("%s", errStr);
		//monitorLog.print(errStr);
		return -1;
	}
	client->flgTransfer_R2S = false;
	return 0;
}

//------------------------------
//icm
//------------------------------

int myFIFO_ICM::myRead(double *rData)
{
	int count;
	char tmp[256];
	len = read(fd, buf, sizeof(buf) - 1);
	if (len == 0)
	{
		//break;
		return 0;
	}
	else
	{
		buf[len] = '\0';
		int i;
		for (i = len - 2; i >= 0; i--) //len-1は最終データの[,]を指してしまうので不可
		{
			if (buf[i] == ',')
				break;
		}
		count = 0;
		for (int j = i + 1; j < len; j++)
		{
			tmp[count] = buf[j];
			count++;
		}
		len = count - 1;

		tmp[len] = '\0';
		*rData = std::strtod(tmp, NULL);
		//if (*rData < 0)
		//			*rData += 360.0;
		//		*rData=360.0-*rData;
		return 1;
	}
}

myFIFO_ICM::myFIFO_ICM()
{
	/*
	int ret = mkfifo(FIFO, 0666);
	if (ret == -1)
	{
		perror("mkfifo");
	}
	*/
	if ((fd = open(FIFO, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("open");
		exit(-1);
	}
	read(fd, buf, sizeof(buf) - 1); //残りバッファを読み込んで置く
}

myFIFO_ICM::~myFIFO_ICM()
{
	close(fd);
	//system("rm -f " FIFO);
}
