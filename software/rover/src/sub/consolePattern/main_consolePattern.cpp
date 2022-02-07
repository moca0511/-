#include "tcpServerClient/consoleClient.h"
#include <iostream>

const std::vector<std::vector<std::string>> strList = {
	{},
	{},
	{},
	{
		//"1 1 1000 0",
		//"255 255 999 0",
		"50 128 999 0",
		"1 128 999 0",
		"128 128 888 0",
	},
	{
		//"4 0 0 100 0 0",
		//"1 35.04569267158931 135.77194960138794 0 0 0"

		"2 0 0 1.5 180 0",
		"2 0 0 1 270 0",
		"2 0 0 1.5 0 0",
		"2 0 0 1 90 0",

	},
	{
		"2 1000 0",
		"3 999 0",
		"4 998 0",
		"5 997 0",
		"2 996 0",
		"3 995 0",
	}};

int main(int argc, char *argv[])
{
	tcpStruct clientS, serverR;
	consoleTcpClient v;
	int sd;
	if (argc != 2)
	{
		printf("option : sendType\n");
		return 0;
	}
	const char *hostnm = ADDR__MY_TCP_SERVER;
	const char *portnm = MY_TCP_POT;
	clientS.flgMonitor = false;
	strcpy(clientS.userName, "roverConsole_pattern");

	clientS.dataType = atoi(argv[1]);

	sd = myTcpConnect(hostnm, portnm);
	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}

	for (int i = 0; i < strList[clientS.dataType].size(); i++)
	{
		//v.printConsoleSupport(clientS.dataType);
		//入力
		v.char2tcpStruct(&clientS, strList[clientS.dataType][i].c_str()); //入力を構造体に代入

		switch (clientS.dataType)
		{
		case 3:
		case 4:
		case 5:
			if (i + 1 == strList[clientS.dataType].size())
				clientS.queueAction = QUEUE__PUSH_AND_POP_ALLOWED;
			else
				clientS.queueAction = QUEUE__PUSH_AND_POP_DISALLOWED;
			break;
		default:
			clientS.queueAction = QUEUE__DO_NOW_AND_STACK_REMOVE;
			break;
		}
		strcpy(clientS.buf, v.tcpStruct2string(&clientS).c_str()); //構造体のデータを表示形式に変換
		printf("send:[name=%s][dataType=%d]%s\n", clientS.userName, clientS.dataType, clientS.buf);
		if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
			return -1;										  //err
	}

	while (clientRecv(sd, &serverR, sizeof(tcpStruct)) == 1)
		;
}
