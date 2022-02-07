#include "readBluetooth.h"
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define CONECT_CHECK_DATA 201
#define SEND_COUNT_MAX 100
#define CHECHK_COUNT_MAX 100 * 10

#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

#define RFCOMM_PATH "/dev/rfcomm1"

myBluetooth::myBluetooth()
{
	if (!checkFileExistence(RFCOMM_PATH))
	{
		printf("rfcommファイルの作成\n");
		char temp[50];
		sprintf(temp, "sudo rfcomm bind 1 %s", BLUETOOTH_ADDR);
		system(temp);
	}
	sleep(1);
	fd = serialOpen(RFCOMM_PATH, 9600);
	wiringPiSetup();
	fflush(stdout);
	if (fd < 0)
		printf("can not open serialport");
}

bool myBluetooth::checkFileExistence(const char str[])
{
	std::ifstream ifs(str);
	return ifs.is_open();
}

int myBluetooth::myRead()
{
	int temp = -1;
	while (serialDataAvail(fd))
	{
		temp = serialGetchar(fd);
		if (temp == -1)
			return -1;
	}
	if (temp < 0)
		return -2;
	sendFlg = 0;
	get_char = temp;
	return get_char;
};

void myBluetooth::mySend(const int data)
{
	checkFlg = 0;
	serialPutchar(fd, data);
	if (sendFlg >= 1 + SEND_COUNT_MAX)
		sendFlg = 0;
	sendFlg++;
	return;
}

int myBluetooth::mySend_check()
{
	checkFlg++;
	if (checkFlg >= CHECHK_COUNT_MAX)
	{
		mySend_check_force();
		return 1;
	}
	return 0;
}

void myBluetooth::mySend_check_force()
{
	serialPutchar(fd, CONECT_CHECK_DATA);
	checkFlg = 0;
}
