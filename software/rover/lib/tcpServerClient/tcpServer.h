#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

/*
参考
https://www.infra.jp/programming/network_programming_1.html#socket
https://www.infra.jp/programming/network_programming_3.html
*/

#include <unistd.h>
#include "tcpStruct.h"

#include <sys/socket.h>
#define CHILD_MAX 10

class my_tcp_server
{
private:
	int soc;
	int server_sock(const char *portnm);
	void childClose(int fdId);
	bool printMode;

public:
	fd_set mask;
	int width;
	int child[CHILD_MAX];
	int sendFlg[CHILD_MAX];
	int readFlg[CHILD_MAX];
	int tcpServerSetup(const char *port);
	void loopInit();

	void newConnect();
	int mySend(int fdId, void *buf, int bufsize);
	int myRecv(int fdId, void *buf, int bufsize);
	void myClose();
	void setPrintMode(bool num);
	void myPrint(const char str[], bool important = false);
};

int mainTcpServer(bool flgPrintMode, bool endMode = false, int endNum = 0);

#endif
