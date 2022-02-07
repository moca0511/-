#ifndef _F9P_TCP_H_
#define _F9P_TCP_H_

#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa(), inet_aton()

//文字列の確保分
#define MSGSIZE 500 //tcpRawData
#define BUFSIZE (MSGSIZE + 1)
#define STR_SIZE 100 //読み込んだ文字列分

#define STR_NMEA_START "$G" //NMEAフォーマットが始まるかの確認用

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
	void hostname2ip(const char hostname_str[], char ip_str[]);
	void myConnect(const char IP_str[], const char port_str[]);

	int ascCheck(char c); //必要無くなったと思う
	char *myStrtok(char *s1, char *s2);
	char *myStrStr(char buff[], const char str[], int strSize);

	int read();
	void scan();

	char recvBuffer[BUFSIZE]; //receive temporary buffer
	int nNum;				  //recvBufferにおける改行文字までの文字数
	char lStr[STR_SIZE];	  //myStrtokS()を実行することで、readBuffer[]から区切り文字がNULLに変わった文字列が保存される。

public:
	F9P_TCP();
	char *sStr[F9P_STR_ELEMENT]; //lStr[]のどの場所をさすかを示す。
	int sStrCount;				 //sStr[]の何番目の要素まで埋まっているかを示す。
	char readBuffer[STR_SIZE];

	void access(const char hostname_str[], const char port_str[]);
	void scanRead()
	{
		read(); //データ受信
		scan(); //受信データを読み込みやすい形に変更
	};
	int checkNmeaType();
	void myStrtokS();
	double convStr_nmea2google(int);
};

#endif
