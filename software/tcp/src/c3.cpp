#include "myClient/tcpClient.h"
tcpStruct clientS, serverR;

int main(int argc, char *argv[])
{
	int sd;
	if (argc != 4)
	{
		printf("option : address port userName\n");
		return 0;
	}
	char *hostnm = argv[1];
	char *portnm = argv[2];

	strcpy(clientS.userName, argv[3]);
	//char *ptr;
	int j = 0;

	sd = myTcpConnect(hostnm, portnm);

	printf("常に入力を受け付けていますが、基本的に'\\n'を入力するまで入力した文字が表示されません。\n");
	printf("また、'\\n'入力時にサーバーにデータを送信します。\n\n");

	while (1)
	{
		switch (clientRecv(sd, &serverR, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
		{
		case 1:
			printf("received:from:%s,msg:%s\n", serverR.userName, serverR.buf);
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
			clientS.buf[j] = '\0';
			printf("send:[%s]%s\n", clientS.userName, clientS.buf);
			if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
				return -1;										 //err

			j = 0;
		}
		/*
		TCP通信と並列で動作を行う。
		ブロッキングを行うとブロックしている間はTCPの送受信ができないので注意。		
		sleep(10);		
		*/
	}
}
