#include "myClient/tcpClient.h"
tcpStruct clientS, serverR;

int main(int argc, char *argv[])
{
	char printBuf[50];
	int sd;
	if (argc != 5)
	{
		printf("option : address port userName dataType\n");
		return 0;
	}
	char *hostnm = argv[1];
	char *portnm = argv[2];

	strcpy(clientS.userName, argv[3]);
	//char *ptr;
	int j = 0;

	if (argv[4][0] == '0')
	{
		clientS.dataType = 0;
	}
	else if (argv[4][0] == '1')
	{
		clientS.dataType = 1;
	}
	else if (argv[4][0] == '2')
	{
		clientS.dataType = 2;
	}
	else
	{
		printf("msgTypeを0~2で選択してください。\n");
		return 1;
	}

	sd = myTcpConnect(hostnm, portnm);

	printf("常に入力を受け付けていますが、基本的に'\\n'を入力するまで入力した文字が表示されません。\n");
	printf("また、'\\n'入力時にサーバーにデータを送信します。\n\n");
	int count = 0;
	while (1)
	{
		switch (clientRecv(sd, &serverR, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
		{
		case 1:

			if (serverR.dataType == 0)
			{
				sprintf(printBuf, "[msgType=%d]%d", serverR.dataType, serverR.useData.data1.num1);
			}
			else if (serverR.dataType == 1)
			{
				sprintf(printBuf, "[msgType=%d]%0.2lf", serverR.dataType, serverR.useData.data2.num2);
			}
			else if (serverR.dataType == 2)
			{
				sprintf(printBuf, "[msgType=%d]%d", serverR.dataType, serverR.useData.data3.num3);
			}

			printf("received:from:%s, %s, msg:%s\n", serverR.userName, printBuf, serverR.buf);
			break; //読み込み成功
		case 0:
			break; //状態変化なし。
		case -1:
		case -2:
			return -1; //err
		default:
			break; //err
		}
		if (kbhit())
		{
			clientS.buf[j] = getchar();
			j++;
		}
		if (clientS.buf[j - 1] == '\n')
		{
			count++;
			if (clientS.dataType == 0)
			{
				clientS.useData.data1.num1 = count;
			}
			else if (clientS.dataType == 1)
			{
				clientS.useData.data2.num2 = count * 1.1;
			}
			else if (clientS.dataType == 2)
			{
				clientS.useData.data3.num3 = count;
			}

			clientS.buf[j] = '\0';
			printf("send:[name=%s][dataType=%d]%s\n", clientS.userName, clientS.dataType, clientS.buf);
			if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
				return -1;										  //err

			j = 0;
		}
		/*
		TCP通信と並列で動作を行う。
		ブロッキングを行うとブロックしている間はTCPの送受信ができないので注意。		
		sleep(10);		
		*/
	}
}
