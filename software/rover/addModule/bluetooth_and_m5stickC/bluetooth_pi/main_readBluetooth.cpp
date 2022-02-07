//g++ main_readBluetooth.cpp readBluetooth.cpp -lwiringPi

#include <iostream>
#include <unistd.h>
#include "readBluetooth.h"

int main()
{
	/* シリアルポートオープン */
	myBluetooth m5stikC;
	while (1)
	{
		/* 受信処理 */
		int d = m5stikC.myRead();
		if (d == -1)
		{
			printf("no data,%d\n", d);
			return 1;
		}
		else if (d == -2)
		{
		}
		else
		{
			printf("recive : %d\n", d);
		}

		if (d % 10 == 0)
		{
			if (m5stikC.getSendFlg() == 0)
			{
				m5stikC.mySend(d);
				printf("send\n");
			}
		}
		else
		{
			if (m5stikC.mySend_check() == 1)
				printf("send_c\n");
		}

		usleep(1000);
		//sleep(1);
	}
	return 0;
}
