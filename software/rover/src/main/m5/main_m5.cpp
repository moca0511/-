#include "tcpServerClient/consoleClient.h"
#include <iostream>

#include "bluetooth_m5stikC/readBluetooth.h"
#include "pastTime/pastTime.h"
#include <unistd.h>
#include "moter/roverMoter.h"

int consoleM5stickC(int n_time = 100);
int main(int argc, char *argv[])
{
	consoleM5stickC();
	return 0;
}

int consoleM5stickC(int n_time)
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

	while (1)
	{
		myBluetooth m5stikC;
		int bMop = M_OPERATION_S;
		while (1)
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
