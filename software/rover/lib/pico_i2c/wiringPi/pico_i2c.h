#ifndef _PICO_I2C_H_
#define _PICO_I2C_H_

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
*/

#include <wiringPiI2C.h>
#include <unistd.h>

#define PICO_ADRESS 0x08

#define TOF_STATES__NORMAL 0
#define TOF_STATES__ERR_CONNECTION 1

#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3

#define I2C_DAT_LOG_COUNT 3
#define I2C_DAT_NUM_COUNT 6
class pico
{
private:
	int fdPICO;
	int dat[I2C_DAT_LOG_COUNT][I2C_DAT_NUM_COUNT];
	int tofConnectNum, tofStates, wallFlg[4];
	int myRead_noise_cancel(unsigned char reg, unsigned char num);

	bool connectFlg = true;
	int readI2C(int num)
	{
		//usleep(10);
		wiringPiI2CWrite(fdPICO, num);
		int a = wiringPiI2CRead(fdPICO);
		if (a == 51)
			a = 0;
		return a;
	};

public:
	void init_p();
	pico();
	int checkConnect();
	int getI2C();
	int getStates() { return tofStates; };
	int getConnectNum() { return tofConnectNum; };
	int getWallFlg(int num) { return wallFlg[num]; };
};

#endif
