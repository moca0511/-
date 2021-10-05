#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "../common/tcpStruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <netinet/in.h>

#include <ctype.h>
#include <cerrno>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <netdb.h>

#include <sysexits.h>
#include <arpa/inet.h>

int myTcpConnect(const char *hostnm, const char *portnm);
int clientRecv(int sd, void *buf, int bufSize);
int clientSend(int sd, void *buf, int bufSize);
#endif
