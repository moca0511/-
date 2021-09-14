#include "tcpLib/myClient/tcpClient.h"
#include <string.h>
#include <iostream>
tcpStruct clientS, serverR;

int main(int argc, char *argv[])
{
	char printBuf[50];
	int sd;
	if (argc != 3)
	{
		printf("option : myTcpServerAddress\n");
		return 0;
	}
	char *hostnm = argv[1];
	const char *portnm = MY_TCP_POT;

	strcpy(clientS.userName, "monitor");
	int j = 0;
	clientS.dataType = 0;
	clientS.flgMonitor = false;

	sd = myTcpConnect(hostnm, portnm);

	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}

	printf("tcpStructのflgMonitorがtrueになっているデータを表示\n");
	int count = 0;
	while (1)
	{
		switch (clientRecv(sd, &serverR, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
		{
		case 1:
			if (serverR.flgMonitor)
			{
				printf("%s", serverR.buf);
			}

			break; //読み込み成功
		case 0:
			break; //状態変化なし。
		case -1:
		case -2:
			return -1; //err
		default:
			break; //err
		}
	}
	return 0;
}
