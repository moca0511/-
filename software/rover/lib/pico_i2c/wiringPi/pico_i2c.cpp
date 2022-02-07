#include "pico_i2c.h"
#include <iostream>

pico::pico()
{
	tofConnectNum = 0;
	tofStates = TOF_STATES__NORMAL;

	tofConnectNum = 0;
	tofStates = 0;
	for (int i = 0; i < 4; i++)
		wallFlg[i] = 0;
}

void pico::init_p()
{
	fdPICO = wiringPiI2CSetup(PICO_ADRESS);
	printf("dd=%d\n", readI2C(0));
}

int pico::myRead_noise_cancel(unsigned char reg, unsigned char num)
{
	int d;
	d = readI2C(reg);

	for (int i = I2C_DAT_LOG_COUNT - 1; i > 0; i--)
	{
		dat[i][num] = dat[i - 1][num];
	}
	dat[0][num] = d;

	int i, j, cnt, mode;
	int temp_mode, temp_cnt;
	mode = 0;
	cnt = 0;
	for (i = 0; i < I2C_DAT_LOG_COUNT; i++)
	{
		temp_cnt = 1;
		for (j = i + 1; j < I2C_DAT_LOG_COUNT; j++)
		{
			if (dat[i][num] == dat[j][num])
			{
				temp_cnt++;
			}
		}
		if (temp_cnt > cnt)
		{
			cnt = temp_cnt;
			mode = dat[i][num];
		}
	}

	if (cnt == 1)
		return -1;

	return mode;
}

int pico::checkConnect()
{
	unsigned char d;
	return 0; //myRead(0, &d);
}

int pico::getI2C()
{
	tofConnectNum = readI2C(0);
	usleep(10);
	tofStates = readI2C(1);
	usleep(10);
	for (int i = 0; i < 4; i++)
	{
		wallFlg[i] = readI2C(i + 2);
		usleep(10);
	}
	return 0;
}

/*
int pico::getI2C()
{
	int i = 0;
	usleep(10);
	tofConnectNum = myRead_noise_cancel(0, 0);
	usleep(10);

	if (connectFlg == false)
		return 1;
	tofStates = myRead_noise_cancel(1, 1);
	usleep(10);
	if (connectFlg == false)
		return 2;

	for (int i = 0; i < 4; i++)
	{
		wallFlg[i] = myRead_noise_cancel(i + 2, i + 2);
		usleep(10);
		if (connectFlg == false)
			return (3 + i);
	}
	return 0;
}
*/
