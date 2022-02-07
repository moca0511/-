//g++ -o write.out main_write.cpp ../fifo.cpp  -I/home/pi/samba/pro4/rover/lib -L/home/pi/samba/pro4/rover/obj/lib  -ltcpServerClient

#include "fifo/fifo.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

int main(int argc, char const *argv[])
{
	FIFO_ROVER rover;

	type_StoR r;
	type_RtoS w;
	tcpStruct d;
	while (1)
	{
		if (rover.read_fifo_fromClient(&d) == 1)
		{
			printf("read : %d\n", d.dataType);
		}

		if (kbhit())
		{
			std::string inputStr;
			printf("input: d> ");
			getline(std::cin, inputStr);
			sscanf(inputStr.c_str(), "%d", &w.rtk);
			printf("get : %d\n", w.rtk);
			rover.write_fifo_toClient(w);
			printf("sendOk\n");
		}
	}
	return 0;
}
