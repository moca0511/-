// g++ main.cpp INA260.cpp -lwiringPi

#include "INA260/INA260.h"
#include <unistd.h>
#include "logFile/myLog.h"

void main_inaLog(const bool flgPrint = true)
{
	char temp[30];
	INA INA_1(INA_PIN_GNA, INA_PIN_GNA, false);
	if (INA_1.getErrFlg() == -1)
	{
		printf("INA:connect err\n");
		return;
	}
	myLogFile LOG_INA("./log/INA_1.log", false);
	LOG_INA.printSimple("\n");
	while (1)
	{
		sprintf(temp, "0x%x, %4.2fv, %4.2fv, %4.2fw\n", INA_1.getAdress(), INA_1.get_V(), INA_1.get_mA(), INA_1.get_P());
		if (flgPrint)
			printf("%s", temp);
		LOG_INA.print(temp);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	main_inaLog(false);
}
