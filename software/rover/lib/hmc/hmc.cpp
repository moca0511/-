#include "hmc.h"
#include <math.h>

int fdHMC;
double deffHmc = 0;

#define HMC_MAX 360.0

void initHMC(int num, double data)
{
	double temp;
	fdHMC = wiringPiI2CSetup(0x21);
	switch (num)
	{
	case HMC_DEFAULT_VALUE__ARGUMENT:
		temp = data;
		break;
	case HMC_DEFAULT_VALUE__MEASURE:
		temp = getHMC();
		break;
	case HMC_DEFAULT_VALUE__NO_USE:
		temp = 0;
		break;
	default:
		break;
	}

	if (temp != -1)
	{
		deffHmc = temp;
		printf("start direction : %.1f\n", deffHmc);
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
	if ((0.0 <= data) && (data <= HMC_MAX))
	{
		data2 = data - deffHmc + HMC_MAX;
		data2 = fmod(data2, HMC_MAX);
		return data2;
	}
	return -1;
}

void printHmc()
{
	printf("%5.1f\n", getHMC());
}

void calibrationHmc(int num = 60)
{
	int fdHMC;
	fdHMC = wiringPiI2CSetup(0x21);
	printf("calibration start\n");
	wiringPiI2CWrite(fdHMC, 0x43);
	for (int i = num; i >= 0; i--)
	{
		printf("%d\n", i);
		sleep(1);
	}
	wiringPiI2CWrite(fdHMC, 0x45);
	printf("calibration end\n");
}
