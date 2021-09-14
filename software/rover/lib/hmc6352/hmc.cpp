#include "hmc.h"
#include <math.h>

int fdHMC;
double deffHmc = 0;

void initHMC()
{
	double temp;
	fdHMC = wiringPiI2CSetup(0x21);
	temp = getHMC();
	if (temp != -1)
	{
		//deffHmc = temp;
		printf("start direction\n : %f", deffHmc);
	}
}

double getHMC()
{
	worder temp, temp2;
	double data, data2;
	//sleep(1);
	//printf("\n");

	temp.word = wiringPiI2CReadReg16(fdHMC, 0x41);
	//usleep(70);
	//temp.word = 0x1234;
	//data = temp.word / 10;
	//printf("0x%04x, %f\n", temp.word, data);

	temp2.lohi.hi = temp.lohi.lo;
	temp2.lohi.lo = temp.lohi.hi;
	data = ((double)temp2.word) / 10.0;
	//printf("0x%x, %f\n", temp2.word, data);
	if ((0.0 <= data) && (data <= 360.0))
	{
		data2 = data - deffHmc + 360.0;
		data2 = fmod(data2, 260.0);
		return data2;
	}
	return -1;
}

void printHmc()
{
	printf("%5.1f\n", getHMC());
}

void calibrationHmc()
{
	int fdHMC;
	fdHMC = wiringPiI2CSetup(0x21);
	printf("calibration start\n");
	wiringPiI2CWrite(fdHMC, 0x43);
	for (int i = 60; i >= 0; i--)
	{
		printf("%d\n", i);
		sleep(1);
	}
	wiringPiI2CWrite(fdHMC, 0x45);
	printf("calibration end\n");
}
