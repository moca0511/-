#include "tcpServerClient/tcpClient.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#define RECV_JSON "/tmp/send.json"
void writeSendJson(int, double, double);
void writeSendJson(const char str[]);

tcpStruct clientS, serverR;

int main(int argc, char *argv[])
{
	const char *readUserName = "rover";
	bool flgUsingUserName = false;
	char printBuf[50];
	int sd;

	const char *hostnm = ADDR__MY_TCP_SERVER;
	const char *portnm = MY_TCP_POT;

	strcpy(clientS.userName, "transfer_R2S");

	int j = 0;
	clientS.dataType = 0;
	clientS.flgMonitor = false;

	sd = myTcpConnect(hostnm, portnm);

	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}
	printf("tcpStructのflgTransfer_R2Sがtrueになっているデータを表示\n");
	int count = 0;
	//printf("read user=%s", readUserName);
	while (1)
	{
		switch (clientRecv(sd, &serverR, sizeof(tcpStruct))) //データ受信(ブロッキングなし)
		{
		case 1:
			if (serverR.flgTransfer_R2S)
			{
				printf("%s\n", serverR.buf);
				writeSendJson(serverR.buf);
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

void writeSendJson(int a, double b, double c)
{

	FILE *fp;

	//「fprintfExample.txt」のファイルを書き込みモード（"w"）で開く
	fp = fopen(RECV_JSON, "w");

	//「fprintfExample.txt」に「aは100です」と書き込む

	char tmp[256];
	sprintf(tmp, "{\
        \"mode\": %d,\
        \"lat\": %f,\
        \"lng\": %f\
		}",
			a, b, c);
	fprintf(fp, tmp);

	//「fprintfExample.txt」のファイルを閉じる
	fclose(fp);
}

void writeSendJson(const char str[])
{

	FILE *fp;

	//「fprintfExample.txt」のファイルを書き込みモード（"w"）で開く
	fp = fopen(RECV_JSON, "w");

	//「fprintfExample.txt」に「aは100です」と書き込む

	fprintf(fp, str);

	//「fprintfExample.txt」のファイルを閉じる
	fclose(fp);
}
