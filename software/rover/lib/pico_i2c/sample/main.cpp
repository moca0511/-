//https://penkoba.hatenadiary.org/entry/20131222/1387689702

#include "pico_i2c/pico_i2c.h"

#include <stdio.h>
#include <iostream>
#include <unistd.h>
int main(int argc, char **argv)
{
	pico myPico;
	if (myPico.getI2C() != 0)
	{
		printf("err\n");
		return 0;
	}
	while (1)
	{
		myPico.getI2C();
		printf("%d, %d, ", myPico.getConnectNum(), myPico.getStates());
		for (int i = 0; i < 4; i++)
		{
			printf("%d, ", myPico.getWallFlg(i));
		}
		printf("\n");
		usleep(100 * 1000);
	}
}
/*
int main(int argc, char **argv)
{
	int i2cMode = 0;
	unsigned char reg, dat;
	i2c myI2c;
	if (argc != 4 && argc != 5)
	{
		fprintf(stderr, "i2cwrite <mode> <slave> <reg> (dat)\n");
		return 1;
	}

	if (argv[1][0] == 'r')
	{
		i2cMode = I2C_READ;
	}
	else if (argv[1][0] == 'w')
	{
		i2cMode = I2C_WRITE;
	}
	else
	{
		printf("cannt select i2c mode. (r or w)\n");
	}

	myI2c.setup((unsigned char)strtol(argv[2], NULL, 0));
	reg = (unsigned char)strtol(argv[3], NULL, 0);
	if (argc == 5)
		dat = (unsigned char)strtol(argv[4], NULL, 0);

	if (i2cMode == I2C_WRITE)
	{
		myI2c.myWrite(reg, dat);
	}
	else
	{
		myI2c.myRead(reg, &dat);
		printf("0x%02x\n", dat);
	}

	return 0;
}
*/
