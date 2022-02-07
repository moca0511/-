#include "tcpServerClient/consoleClient.h"
#include <string.h>
#include <iostream>

int main(int argc, char *argv[])
{
	consoleTcpClient v;
	tcpStruct clientS, serverR;
	int sd;
	if (argc != 2)
	{
		printf("option : sendType\n");
		return 0;
	}
	const char *hostnm = ADDR__MY_TCP_SERVER;
	const char *portnm = MY_TCP_POT;


	init_tcpStruct(&clientS, "roverConsole");
	clientS.dataType = atoi(argv[1]);

	sd = myTcpConnect(hostnm, portnm);
	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}

	printf("常に入力を受け付けていますが、基本的に'\\n'を入力するまで入力した文字が表示されません。\n");
	printf("また、'\\n'入力時にサーバーにデータを送信します。\n\n");

	while (1)
	{
		switch (clientRecv(sd, &serverR, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
		{
		case 1:
			if (serverR.flgMonitor)
			{
				printf("%s", serverR.buf);
			}
			else
			{
				consoleTcpClient v;
				strcpy(serverR.buf, v.tcpStruct2string(&serverR).c_str()); //構造体のデータを表示形式に変換
				printf("received:from:%s, msg:%s\n", serverR.userName, serverR.buf);
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
		if (kbhit())
		{
			int t;
			std::string inputStr;
			t = getchar();
			clientS.buf[0] = '\0';
			v.printConsoleSupport(clientS.dataType);													//入力補助データ表示
			getline(std::cin, inputStr);																//入力
			v.char2tcpStruct(&clientS, inputStr.c_str());												//入力を構造体に代入
			strcpy(clientS.buf, v.tcpStruct2string(&clientS).c_str());									//構造体のデータを表示形式に変換
			printf("send:[name=%s][dataType=%d]%s\n", clientS.userName, clientS.dataType, clientS.buf); //表示
			if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1)										//データ送信実行
				return -1;																				//err
			printf("\n");
			//kbhit();
			//getchar();
		}

		/*
		TCP通信と並列で動作を行う。
		ブロッキングを行うとブロックしている間はTCPの送受信ができないので注意。		
		sleep(10);		
		*/
	}
}

/*
0 35.472222 133.050556 0 0
1 0 0 21.21518366 490.58216516
*/
