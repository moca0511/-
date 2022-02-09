#ifndef TCP_H
#define TCP_H

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

#define MSGSIZE 1024
#define BUFSIZE (MSGSIZE + 1)

#define GNVTG_STR "$GNVTG"

#define MODE_READ_SPEED 1
#define MODE_READ_RESULT 2

class tcpClient
{
private:
	char recvBuffer[BUFSIZE];		   //receive temporary buffer
	char sendBuffer[BUFSIZE] = "\n\0"; // send temporary buffer

	int sock;						 //local socket descriptor
	struct sockaddr_in servSockAddr; //server internet socket address
	
	unsigned short servPort;		 //server port number

	char readBuffer[BUFSIZE];
	
	int ascCheck(char c);
	char *myStrtok(char *s1, char *s2);

	char readFlg = 0; //0=true,1>=false

	void scanG();
	void scanRaw();

	void printG_strtokc(int printFlg = 1);
	void printR_strtokc(int printFlg = 1);

public:
	tcpClient(int mode);

	char nowMode;

	double angele = 0; //GPVTGから読み取った地表における移動の磁方位
	double speed = 0;  //GPVTGから読み取った地表における移動の速度(0000.0?1800.0[km/h])

	double lat;
	double lon;
	double hei;
	double ratio;

	void hostname2ip(char hostname_str[], char ip_str[]);
	void myConnect(char IP_str[], char port_str[]);

	void access(char hostname_str[], char port_str[]);
	int read();
	void print();
	void setMode(char wantMode);
	void scan();
	void print_strtokc(int printFlg = 1);
};

#endif
