#include "f9p_tcp.h"

#include <stdio.h>		//printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
//#include <arpa/inet.h>	// struct sockaddr_in, struct sockaddr, inet_ntoa(), inet_aton()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset(), strcmp()
#include <unistd.h> //close()

#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <math.h>

F9P_TCP::F9P_TCP()
{
	for (int i = 0; i < F9P_STR_ELEMENT; i++)
	{
		sStr[i] = NULL;
	}
}

//ホスト名とIPアドレスから接続をTCP行う  (*未テスト)
//引数1=ホスト名(IPアドレス)、引数2=ポート番号
void F9P_TCP::access(const char hostname_str[], const char port_str[])
{
	char temp[256];
	hostname2ip(hostname_str, temp);
	myConnect(temp, port_str);
}

/*
strtok関数の区切り文字の連続を見落とさないバージョン
*/
char *F9P_TCP::myStrtok(char *s1, char *s2)
{
	static char *str = 0;

	if (s1)
	{
		str = s1;
	}
	else
	{
		s1 = str;
	}
	if (!s1)
		return (0);

	while (1)
	{
		if (!*str)
		{
			str = 0;
			return (s1);
		}

		if (*str == *s2)
		{
			*str++ = 0;
			return (s1);
		}

		str++;
	}
}

//アスキーコードの確認
//戻り値：1=失敗、o=成功
int F9P_TCP::ascCheck(char c)
{
	if ((c >= 0x20) && (c < 0x7f))
	{
		return 0;
	}
	return 1;
}

//ホスト名からIPアドレス取得
//引数1==ホスト名(IPアドレス)、引数2=検索結果保存用文字列
void F9P_TCP::hostname2ip(const char hostname_str[], char ip_str[])
{
	struct sockaddr_in addr;
	struct hostent *host;
	host = gethostbyname(hostname_str);
	if (host != NULL)
	{
		addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
	}
	sprintf(ip_str, "%s", inet_ntoa(addr.sin_addr));
}

//IPアドレスとポート番号を参考にTCP接続
//引数1=IPアドレス、引数2=ポート番号
void F9P_TCP::myConnect(const char IP_str[], const char port_str[])
{
	memset(&servSockAddr, 0, sizeof(servSockAddr));

	servSockAddr.sin_family = AF_INET;

	if (inet_aton(IP_str, &servSockAddr.sin_addr) == 0)
	{
		fprintf(stderr, "Invalid IP Address.\n");
		exit(EXIT_FAILURE);
	}

	if ((servPort = (unsigned short)atoi(port_str)) == 0)
	{
		fprintf(stderr, "invalid port number.\n");
		exit(EXIT_FAILURE);
	}
	servSockAddr.sin_port = htons(servPort);

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr *)&servSockAddr, sizeof(servSockAddr)) < 0)
	{
		perror("f9p connect failed");
		exit(EXIT_FAILURE);
	}

	//printf("connect to %s\n", inet_ntoa(servSockAddr.sin_addr));
}

//接続済みTCPソケットから文字列を読み取る
int F9P_TCP::read()
{
	char sendBuffer[BUFSIZE] = "\n\0"; // send temporary buffer

	if (send(sock, sendBuffer, strlen(sendBuffer), 0) <= 0)
	{
		perror("send() failed.");
		exit(EXIT_FAILURE);
	}

	int byteRcvd = 0;
	int byteIndex = 0;
	nNum = 0;
	while (byteIndex < MSGSIZE)
	{
		byteRcvd = recv(sock, &recvBuffer[byteIndex], 1, 0);
		if (byteRcvd > 0)
		{

			if (recvBuffer[byteIndex] == '\n')
			{
				nNum = byteIndex;
				recvBuffer[byteIndex] = '\0';
				break;
			}
			else if (recvBuffer[byteIndex] == '\r')
			{
				nNum = byteIndex;
				recvBuffer[byteIndex] = '\0';
				//break;
			}

			byteIndex += byteRcvd;
		}
		else if (byteRcvd == 0)
		{
			perror("ERR_EMPTY_RESPONSE");
			exit(EXIT_FAILURE);
		}
		else
		{
			perror("recv() failed.");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

//recvBuffer内にある文字列を、NMEAフォーマットを読み取る形式でreadBufferに格納
//ubox形式と連続する一つ目のNMEAフォーマットの文字列は読み取れない(GPRMCなど)
void F9P_TCP::scan()
{
	char *adr;
	int i;
	//adr = strstr(recvBuffer, STR_NMEA_START);
	adr = myStrStr(recvBuffer, STR_NMEA_START, sizeof(STR_NMEA_START));
	//printf("server return: %s\n", recvBuffer);
	if (adr != NULL)
	{
		int j = 0;
		while (*(adr + j) != '\0')
		{
			if (ascCheck(*(adr + j)) == 0)
				readBuffer[j] = *(adr + j);
			else
				readBuffer[j] = ' ';
			j++;
		}
		readBuffer[j] = '\0';
	}
}

char *F9P_TCP::myStrStr(char buff[], const char str[], int strSize)
{

	int i, j, k;

	if (*str == '\0')
		return buff;
	else
	{
		for (i = 0; i < nNum; i++)
		{
			if (*(buff + i) == *str)
			{
				for (j = i, k = 0; *(buff + j) == *(str + k); j++, k++)
					;
				if (*(str + k) == '\0')
					return buff + i;
			}
		}
		return NULL;
	}
}

//readBuffer内の文字列がNMEAフォーマットのどれかを確認
//戻り値:2=GNVTG,1=GNGGA,0=その他
int F9P_TCP::checkNmeaType()
{
	if (strstr(readBuffer, "$GNRMC") != NULL)
	{
		return 2;
	}
	else if (strstr(readBuffer, "$GNGGA") != NULL)
	{
		return 1;
	}
	else if (strstr(readBuffer, "$GNVTG") != NULL)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

//readBuffer内の文字列を区切り文字ごとに認識し、移動方向と移動速度を変数に格納
//引数=1の時、区切り文字で区切った結果を表示、0=区切り文字で区切った結果を非表示
void F9P_TCP::myStrtokS()
{
	char s2[] = ",";
	char *tok;

	sStrCount = 0;
	strcpy(lStr, readBuffer);
	tok = myStrtok(lStr, s2);
	while (tok != NULL)
	{
		sStrCount++;
		sStr[sStrCount] = tok;

		tok = myStrtok(NULL, s2);
	}
}

double F9P_TCP::convStr_nmea2google(int elemnt)
{
	double temp;
	temp = strtod(sStr[elemnt], NULL) / 100.000000000000000;
	temp = floor(temp) + ((temp - floor(temp)) / 60.0) * 100.0;
	return temp;
}
