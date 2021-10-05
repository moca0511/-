#ifndef _F9P_TCP_H_
#define _F9P_TCP_H_

#include <stdio.h>		//printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h>	// struct sockaddr_in, struct sockaddr, inet_ntoa(), inet_aton()
#include <stdlib.h>		//atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>		//memset(), strcmp()
#include <unistd.h>		//close()

//
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//文字列の確保分
#define MSGSIZE 500	//tcpRawData
#define BUFSIZE (MSGSIZE + 1)
#define STR_SIZE 100	//読み込んだ文字列分

#define STR_NMEA_START "$G"	//NMEAフォーマットが始まるかの確認用

//必要であるNMEAフォーマットの何番目の要素であるか
#define RMC_KNOT 8
#define RMC_DIRECTION 9

#define GGA_LAT 3
#define GGA_LON 5
#define GGA_RTK 7

#define F9P_STR_ELEMENT 20

class F9P_TCP
{
private:
	int sock;						 //local socket descriptor
	struct sockaddr_in servSockAddr; //server internet socket address
	unsigned short servPort;		 //server port number
	void hostname2ip(char hostname_str[], char ip_str[]);
	void myConnect(char IP_str[], char port_str[]);



	int ascCheck(char c);	//必要無くなったと思う
	char *myStrtok(char *s1, char *s2);
	char *myStrStr(char buff[], const char str[], int strSize);

	int read();
	void scan();

	char recvBuffer[BUFSIZE]; //receive temporary buffer
	int nNum;				  //recvBufferにおける改行文字までの文字数
	char lStr[STR_SIZE]; //myStrtokS()を実行することで、readBuffer[]から区切り文字がNULLに変わった文字列が保存される。

public:
	char *sStr[F9P_STR_ELEMENT]; //lStr[]のどの場所をさすかを示す。
	int sStrCount;	//sStr[]の何番目の要素まで埋まっているかを示す。
	char readBuffer[STR_SIZE];

	void access(char hostname_str[], char port_str[]);
	void scanRead() {
		read(); //データ受信
		scan(); //受信データを読み込みやすい形に変更
	};

	int checkNmeaType();
	void myStrtokS();
};

#endif
