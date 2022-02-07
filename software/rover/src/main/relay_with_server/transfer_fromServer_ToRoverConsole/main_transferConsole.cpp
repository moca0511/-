#include <stdio.h>
#include <sys/stat.h>

#include <sys/fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <vector>
#include <string.h>
#include <sstream>

#include "tcpServerClient/consoleClient.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define GET_RECV_JSON "/tmp/recv.json"

int myRead(const int fd, void *buf, const int size);
int analaysis_char2json(const char, json *);

int main()
{

	//tcp関係の変数宣言
	tcpStruct clientS, serverR;
	consoleTcpClient v;
	int sd;

	const char *hostnm = ADDR__MY_TCP_SERVER;
	const char *portnm = MY_TCP_POT;
	clientS.flgMonitor = false;
	init_tcpStruct(&clientS, "transferConsole");
	clientS.dataType = 5;
	sd = myTcpConnect(hostnm, portnm);
	if (sd == -1)
	{
		printf("サーバに接続できませんでした。\n");
		return 0;
	}

	//json解析用変数宣言
	char ch;
	int retMyRead, fdR;
	json jobj;

	//読み込みfd準備
	fdR = open(GET_RECV_JSON, O_RDONLY | O_NONBLOCK);
	if (fdR == -1)
	{
		perror("open err fdR");
		exit(0);
	}

	while (1)
	{
		if (clientRecv(sd, &serverR, sizeof(tcpStruct)) < 0)
		{
			return -1;
		}

		retMyRead = myRead(fdR, &ch, 1);
		if (retMyRead != 0)
		{

			if (analaysis_char2json(ch, &jobj) == 1)
			{
				//std::cout << "\nJSONをダンプ: " << jobj.dump() << std::endl; // 整形無しで出力
				std::cout
					<< jobj[json::json_pointer("/mode")] << ", "
					<< jobj[json::json_pointer("/lat")] << ", "
					<< jobj[json::json_pointer("/lng")] << "\n";

				int getMode;
				double getLat, getLng;
				getMode = jobj[json::json_pointer("/mode")].get<int>();
				getLat = jobj[json::json_pointer("/lat")].get<double>();
				getLng = jobj[json::json_pointer("/lng")].get<double>();

				char tmp[100];
				if (getMode != 5)
				{
					clientS.dataType = 5;
					getMode++;
					if (getMode == 1)
					{
						sprintf(tmp, "%d 0 100\0", getMode);
					}
					else
					{
						sprintf(tmp, "%d 0 0\0", getMode);
					}
				}
				else
				{
					clientS.dataType = 4;
					sprintf(tmp, "1 %f %f 0 0 0\0", getMode, getLat, getLng);
				}
				v.char2tcpStruct(&clientS, tmp);
				clientS.queueAction = QUEUE__DO_NOW_AND_STACK_REMOVE;
				if (clientSend(sd, &clientS, sizeof(tcpStruct)) == 1) //データ送信実行
					return -1;										  //err
			}
		}
	}
}

char buf_charTemp[256], countChar = 0;
/*
目的:	[}]になるまでchを読み込み続ける
		[}]を読み込んだらこれまで読み込んだデータを引数jobjに渡す
戻り値:	1=[}]読み込み。0=その他
*/
int analaysis_char2json(const char ch, json *jobj)
{
	buf_charTemp[countChar] = ch;
	countChar++;
	if (ch == '}')
	{
		buf_charTemp[countChar] = '\0';
		countChar = 0;
		std::string bufStr = buf_charTemp;
		*jobj = json::parse(bufStr);
		return 1;
	}
	return 0;
}

//読み込んだデータはbufに代入
//戻り値1は読み込み成功
int myRead(const int fd, void *buf, const int size)
{
	int len = read(fd, buf, size);
	if (len < 0)
	{
		if (errno == EAGAIN)
		{
			//fprintf(stderr, "avoid blocking\n");
		}
		else
			perror("read");
		return 0;
	}
	else if (len == 0)
	{
		//printf("EOF\n"); // len == 0のときはパイプの中身を全て読んでいる状態。
		return 0;
	}
	return 1;
}
