#include "pico_i2c.h"

pico::pico()
{
	tofConnectNum = 0;
	tofStates = TOF_STATES__NORMAL;
	i2cMode = I2C_READ;
	setup(PICO_ADRESS);

	tofConnectNum = 0;
	tofStates = 0;
	for (int i = 0; i < 4; i++)
		wallFlg[i] = 0;
}

int pico::myRead_noise_cancel(unsigned char reg, unsigned char num)
{
	unsigned char d;
	if (myRead(reg, &d) == 1)
		connectFlg = false;

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
	return myRead(0, &d);
}

int pico::getI2C()
{
	int i = 0;
	tofConnectNum = myRead_noise_cancel(0, 0);
	if (connectFlg == false)
		return 1;
	tofStates = myRead_noise_cancel(1, 1);
	if (connectFlg == false)
		return 2;

	for (int i = 0; i < 4; i++)
	{
		wallFlg[i] = myRead_noise_cancel(i + 2, i + 2);
		if (connectFlg == false)
			return (3 + i);
	}
	return 0;
}

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
