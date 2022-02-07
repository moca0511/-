#ifndef _PICO_I2C_H_
#define _PICO_I2C_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_READ 0
#define I2C_WRITE 1

#define I2C_DEV "/dev/i2c-1"

#define PICO_ADRESS 0x08

#define TOF_STATES__NORMAL 0
#define TOF_STATES__ERR_CONNECTION 1

#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3

class i2c
{
private:
	int openFlg = 0;
	int errCount = 0;
	int fd;
	unsigned char slave;
	unsigned char buf[2];
	char i2c_dev_fn[64];

public:
	~i2c();
	int setup(int address);
	int myRead(unsigned char reg, unsigned char *dat);
	int myWrite(unsigned char reg, unsigned char dat);
};

#define I2C_DAT_LOG_COUNT 3
#define I2C_DAT_NUM_COUNT 6

class pico : private i2c
{
private:
	int i2cMode;
	unsigned char dat[I2C_DAT_LOG_COUNT][I2C_DAT_NUM_COUNT];
	int tofConnectNum, tofStates, wallFlg[4];
	int myRead_noise_cancel(unsigned char reg, unsigned char num);

	bool connectFlg = true;

public:
	pico();
	void init_p() { };
	int checkConnect();
	int getI2C();
	int getStates() { return tofStates; };
	int getConnectNum() { return tofConnectNum; };
	int getWallFlg(int num) { return wallFlg[num]; };
};

#endif
