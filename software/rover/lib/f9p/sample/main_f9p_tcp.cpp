#include "f9p/f9p_tcp.h"

#include <stdio.h>		//printf(), fprintf(), perror()
#include <stdlib.h>		//atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS

int f9p_read_totalProcess(F9P_TCP *myF9P)
{
	int stat;
	myF9P->scanRead();
	stat = myF9P->checkNmeaType();
	if (stat != 0)
	{
		myF9P->myStrtokS();
	}
	return stat;
}

int main(int argc, char *argv[])
{

	//引数の数チェック
	if (argc != 3)
	{
		fprintf(stderr, "argument count mismatch error.\n");
		fprintf(stderr, "hostname port\n");
		exit(EXIT_FAILURE);
	}

	//クラスのモード確認
	F9P_TCP myF9P;

	//tcpクラスの接続
	myF9P.access(argv[1], argv[2]);
	printf("start\n");
	int j = 0;
	while (1)
	{
		int stat;
		if ((stat = f9p_read_totalProcess(&myF9P)) != 0)
		{
			printf("get : ");
			printf("%-80s :: ", myF9P.readBuffer);
			printf("%-10s :: ", myF9P.sStr[myF9P.sStrCount]);
			printf("%d :: ", stat);
			if (stat == 2)
			{
				printf("knot=%s, direction=%s", myF9P.sStr[RMC_KNOT], myF9P.sStr[RMC_DIRECTION]);
			}
			else if(stat==1)
			{
				printf("%s, lat=%s, lon=%s", myF9P.sStr[GGA_RTK], myF9P.sStr[GGA_LAT], myF9P.sStr[GGA_LON]);
			}
			printf("\n");
			j++;
		}
		else if (j >= 3)
		{
			j = 0;
			printf("\n");
		}
	}
	return EXIT_SUCCESS;
}
