// g++ main.cpp INA260.cpp -lwiringPi

#include "INA260/INA260.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	INA INA_1(INA_PIN_GNA, INA_PIN_GNA);
	INA INA_2(0x044);
	while (1)
	{
		printf("0x%x, %4.2fv, %4.2fmA, %4.2fw, \t", INA_1.getAdress(), INA_1.get_V(), INA_1.get_mA(), INA_1.get_P());
		printf("0x%x, %4.2fv, %4.2fmA, %4.2fw,", INA_2.getAdress(), INA_2.get_V(), INA_2.get_mA(), INA_2.get_P());
		printf("\n");
		sleep(1);
	}
}
