#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

/*
参考
https://www.infra.jp/programming/network_programming_1.html#socket
https://www.infra.jp/programming/network_programming_3.html
*/

#include "common/tcpStruct.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <ctype.h>
#include <cerrno>
#include <netdb.h>
#include <unistd.h>
#include <sysexits.h>

#define CHILD_MAX 5

extern int child[CHILD_MAX];

extern int soc;
extern fd_set mask;
extern int width;
extern int sendFlg[CHILD_MAX];
extern int readFlg[CHILD_MAX];

int server_sock(const char *portnm);
int tcpServerSetup(const char *port);
void loopInit();

void newConnect();
void childClose(int fdId);
int mySend(int fdId, void *buf, int bufsize);
int myRecv(int fdId, void *buf, int bufsize);
void myClose();
#endif
