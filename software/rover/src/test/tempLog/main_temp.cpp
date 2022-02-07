
#include "logFile/myLog.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>

void main_dataLog()
{
	const char logPath[] = "./log/temp.log";
	const char cmdTr[] = "tr '\\n' ' '  >> ";
	char cmd[100];
	myLogFile LOG_TEMP(logPath, false);
	LOG_TEMP.printSimple("\n");

	while (1)
	{
		LOG_TEMP.print(" ");
		sprintf(cmd, "vcgencmd get_throttled | %s %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);
		LOG_TEMP.printSimple("\t");

		sprintf(cmd, "vcgencmd measure_clock arm | %s %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);
		LOG_TEMP.printSimple("\t");

		sprintf(cmd, "vcgencmd measure_temp | %s  %s", cmdTr, LOG_TEMP.getFileName().c_str());
		system(cmd);

		LOG_TEMP.printSimple("\n");

		sleep(1);
	}
}

int main(void)
{
	main_dataLog();
	return 0;
}
