//g++ -o client.out main_Client.cpp ../fifo.cpp  -I/home/pi/samba/pro4/rover/lib -L/home/pi/samba/pro4/rover/obj/lib  -ltcpServerClient
#include "fifo/fifo.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

int main(int argc, char const *argv[])
{
	//FIFO_ROVER m;
	FIFO_CLIENT c;
	type_StoR w;
	type_RtoS r;
	while (1)
	{
		if (c.read_fifo_fromMaster(&r) == 1)
		{
			printf("read:rtk=%d\n", r.rtk);
		}

		if (kbhit())
		{
			std::string inputStr;
			printf("input: d f f > ");
			getline(std::cin, inputStr);
			sscanf(inputStr.c_str(), "%d %lf %lf", &w.mode, &w.data1, &w.data2);
			printf("get : %d, %f, %f\n", w.mode, w.data1, w.data2);
			c.write_fifo_toMaster(w);
			printf("sendOk\n");
		}
	}
	return 0;
}
