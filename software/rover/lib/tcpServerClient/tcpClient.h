#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "tcpStruct.h"

int myTcpConnect(const char *hostnm, const char *portnm, bool printFlg = true);
int clientRecv(int sd, void *buf, int bufSize, bool printFlg = true);
int clientSend(int sd, void *buf, int bufSize);
#endif
