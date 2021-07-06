//https://penkoba.hatenadiary.org/entry/20131222/1387689702
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

class i2c
{
private:
	int openFlg = 0;

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

i2c::~i2c()
{
	if (openFlg == 1)
	{
		//close
	}
}

int i2c::setup(int address)
{
	slave = address;

	if ((fd = open(I2C_DEV, O_RDWR)) < 0)
	{
		printf("Faild to open i2c port\n");
		return 1;
	}

	if (ioctl(fd, I2C_SLAVE, slave) < 0)
	{
		printf("Unable to get bus access to talk to slave\n");
		return 1;
	}
	openFlg = 1;
	return 0;
}

int i2c::myRead(unsigned char reg, unsigned char *dat)
{
	/* write address */
	if ((write(fd, &reg, 1)) != 1)
	{
		fprintf(stderr, "Error writing to i2c slave\n");
		return 1;
	}

	/* read data */
	if (read(fd, dat, 1) != 1)
	{
		fprintf(stderr, "Error reading from i2c slave\n");
		return 1;
	}
	return 0;
}

int i2c::myWrite(unsigned char reg, unsigned char dat)
{
	buf[0] = reg;
	buf[1] = dat;
	if ((write(fd, buf, 2)) != 2)
	{
		fprintf(stderr, "Error writing to i2c slave\n");
		return 1;
	}
	return 0;
}

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
