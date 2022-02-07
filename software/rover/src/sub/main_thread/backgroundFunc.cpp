#include <iostream>
#include "backgroundFunc.h"
#include "tcpServerClient/tcpServer.h"
#include "bluetooth_m5stikC/readBluetooth.h"
#include "pastTime/pastTime.h"
#include "tcpServerClient/consoleClient.h"
#include "moter/roverMoter.h"
#include "INA260/INA260.h"
#include "logFile/myLog.h"

#include <unistd.h>
uint32_t end_flag_ = 1;

void (*const backgroundFunc[FUNC_NUM])(void) = {
	backgroundServer,
	backgroundF9p,
	backGraundM5,
	backGraundInaLog,
	backGraundTemp,
};

void backgroundServer()
{
	printf("start server\n");
	mainTcpServer(false, true, 1);

	printf("end server\n");
	sleep(1);
	system("pkill str2str");
}

void backgroundF9p()
{
	int ret = system("lsusb | grep U-Blox > /dev/null 2>&1");
	//printf("%d\n", ret);
	//system("echo ${PRO4_ROVER_DIR_PATH}");
	if (ret == 0)
	{
		printf("start str2str\n");
		//system("sh ../f9p/commdschool.sh > /dev/null 2>&1");
		//system("python3 ../f9p/str2str.py");
		system("sh ../f9p/str2str.sh");

		printf("end str2str\n");
	}
	else
	{
		printf("err: start str2str\n");
	}
}

void backGraundM5()
{
	sleep(SERVER_LODING_TIME);
	printf("start m5stickC\n");
	consoleM5stickC(100);
	printf("end m5sticC\n");
}

void backGraundTemp()
{
	printf("start tempLog\n");
	main_tempLog();
	printf("end tempLog\n");
}

void backGraundInaLog()
{
	printf("start InaLog\n");
	main_inaLog(false);
	printf("end InaLog\n");
}

int consoleM5stickC(int n_time = 100)
{
	tcpStruct clientS, serverR;
	consoleTcpClient v;
	myPastTime m5Time, m5Time_check;
	int sd;

	const char *hostnm = ADDR__MY_TCP_SERVER;
	const char *portnm = MY_TCP_POT;
	clientS.flgMonitor = false;
	strcpy(clientS.userName, "roverConsole_m5StikC");

	clientS.dataType = 5;

	sd = myTcpConnect(hostnm, portnm, false);
	if (sd == -1)
	{
		printf("サーバに接続できませんでした。_m5stcikC\n");
		return 0;
	}

	while (end_flag_)
	{
		myBluetooth m5stikC;
		int bMop = M_OPERATION_S;
		while (end_flag_)
		{
			//入力
			if (m5Time.getPastTime() > 100)
			{
				int d = m5stikC.myRead();
				if (d == -1)
				{
					//printf("no data,%d\n", d);
					m5Time_check.setBaselineTime();
					break;
				}
				else if (d == -2)
				{
				}
				else
				{
					clientS.useData.data5.moterOperation = M_OPERATION_S;
					if (d >= 100)
						clientS.useData.data5.moterOperation = d - 100;

					if ((clientS.useData.data5.moterOperation == M_OPERATION_S) && (bMop == M_OPERATION_S))
					{

						m5Time.setBaselineTime();
						continue;
					}
					bMop = clientS.useData.data5.moterOperation;
					clientS.useData.data5.time = n_time;
					clientS.queueAction = QUEUE__DO_NOW_AND_STACK_REMOVE;

					strcpy(clientS.buf, v.tcpStruct2string(&clientS).c_str()); //構造体のデータを表示形式に変換
					//printf("send:[name=%s][dataType=%d]%s\n", clientS.userName, clientS.dataType, clientS.buf);
					if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
						return -1;										  //err
				}
				m5Time.setBaselineTime();
			}

			if (m5Time_check.getPastTime() > 2000)
			{
				m5stikC.mySend_check_force();
				//printf("send_c\n");
				m5Time_check.setBaselineTime();
			}
			int cRev = clientRecv(sd, &serverR, sizeof(tcpStruct), false);
			if (cRev == -2)
			{
				return 2;
			}
			usleep(1000 * 100);
		}
	}
	return 0;
}

void main_inaLog(const bool flgPrint)
{
	char temp[60];
	INA INA_1(INA_PIN_GNA, INA_PIN_GNA, false);
	INA INA_2(0x44, false);
	if (INA_1.getErrFlg() == -1)
	{
		printf("INA_1:connect err\n");
		return;
	}
	if (INA_2.getErrFlg() == -1)
	{
		printf("INA_2:connect err\n");
		return;
	}

	myLogFile LOG_INA("./log/INA_1.log", false);
	LOG_INA.printSimple("\n");
	while (end_flag_)
	{
		sprintf(temp, "0x%x, %4.2fv, %4.2fv,\t0x%x, %4.2fv, %4.2fv\n", INA_1.getAdress(), INA_1.get_V(), INA_1.get_mA(), INA_2.getAdress(), INA_2.get_V(), INA_2.get_mA());
		if (flgPrint)
			printf("%s", temp);
		LOG_INA.print(temp);
		sleep(1);
	}
}

void main_tempLog()
{
	const char logPath[] = "./log/temp.log";
	const char cmdTr[] = "tr '\\n' ' '  >> ";
	char cmd[100];
	myLogFile LOG_TEMP(logPath, false);
	LOG_TEMP.printSimple("\n");

	while (end_flag_)
	{
		LOG_TEMP.print(" ");
		sprintf(cmd, "vcgencmd get_throttled | %s %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);
		LOG_TEMP.printSimple("\t");

		sprintf(cmd, "vcgencmd measure_clock arm | %s %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);
		LOG_TEMP.printSimple("\t");

		sprintf(cmd, "vcgencmd measure_temp | %s  %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);

		LOG_TEMP.printSimple("\n");

		sleep(1);
	}
}
