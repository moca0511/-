#include "tcpLib/myClient/tcpClient.h"
#include <string.h>

tcpStruct clientS, serverR;

int main(int argc, char *argv[])
{
	char printBuf[50];
	int sd;
	if (argc != 3)
	{
		printf("option : address sendType\n");
		return 0;
	}
	char *hostnm = argv[1];
	const char *portnm = MY_TCP_POT;

	strcpy(clientS.userName, "roverConsole");
	//char *ptr;
	int j = 0;
	clientS.dataType = atoi(argv[2]);

	sd = myTcpConnect(hostnm, portnm);

	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}

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
				sprintf(printBuf, "[msgType=%d]%d", serverR.dataType, serverR.useData.data0.num1);
			}
			else if (serverR.dataType == 1)
			{
				sprintf(printBuf, "[msgType=%d]%0.2lf", serverR.dataType, serverR.useData.data1.num2);
			}
			else if (serverR.dataType == 2)
			{
				sprintf(printBuf, "[msgType=%d]%d", serverR.dataType, serverR.useData.data2.num3);
			}
			else if (serverR.dataType == 3)
			{
				sprintf(printBuf, "[msgType=%d] (%3d,%3d)%dms", serverR.dataType, serverR.useData.data3.moterL, serverR.useData.data3.moterR, serverR.useData.data3.time);
				printf("received:from:%s, %s\n", serverR.userName, printBuf);
				break;
			}
			else if (serverR.dataType == 4)
			{
				printf("data type 4\n");
				break;
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
				clientS.useData.data0.num1 = count;
			}
			else if (clientS.dataType == 1)
			{
				clientS.useData.data1.num2 = count * 1.1;
			}
			else if (clientS.dataType == 2)
			{
				clientS.useData.data2.num3 = count;
			}
			else if (clientS.dataType == 3)
			{
				//入力文字列から数字を取り出す
				const char delim[] = ","; // 区切り文字
				char *token;			  // 分離後の文字列を指すポインタ
				char tokenStr[256];		  //編集用配列
				char *temp[6];			  //区切った文字列のポインタを記録
				int tempCount = 0;
				strcpy(tokenStr, clientS.buf);
				token = strtok(tokenStr, delim);
				while (token != NULL)
				{
					// 分離後の文字列を表示
					temp[tempCount] = token;
					// 文字列を分離
					token = strtok(NULL, delim);
					tempCount++;
				}
				if (tempCount != 3)
				{
					printf("文字列の区切りが間違っています[%d]%s\n", tempCount, clientS.buf);
					j = 0;
					continue;
				}
				clientS.useData.data3.moterL = strtol(temp[0], NULL, 10);
				clientS.useData.data3.moterR = strtol(temp[1], NULL, 10);
				clientS.useData.data3.time = strtol(temp[2], NULL, 10);
			}
			else if (clientS.dataType == 4)
			{
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
