#include "INA260.h"

#include <iomanip>
#include <stdlib.h>
#include <math.h>
//------------------------------
// 変数宣言

/* 16bitの共用体 */
typedef union
{
	short int word;
	/* 8bit2個=16bitの構造体 */
	struct
	{
		char lo;
		char hi;
	} lohi;
} worder;
//------------------------------

INA::INA(const int A0, const int A1, const bool flgPrint)
{
	int sd;
	ina_address = 0b1000000 + A0 * 4 + A1;
	sd = init(ina_address);
	if (flgPrint)
		printf("INA260 address:%d, fd=%d, sd=%d\n", ina_address, fdINA, sd);
	if (sd == -1)
		errFlg = sd;
}

INA::INA(const int add, const bool flgPrint)
{
	int sd;
	sd = init(add);
	if (flgPrint)
		printf("INA260 address:%d, fd=%d, sd=%d\n", ina_address, fdINA, sd);
	if (sd == -1)
		errFlg = sd;
	ina_address = add;
}

int INA::init(const int add){
	int sd;
	fdINA = wiringPiI2CSetup(add);
	sd = wiringPiI2CRead(fdINA);
	return sd;
}

double INA::get_Data(const int reg)
{
	worder temp, temp2;

	temp.word = wiringPiI2CReadReg16(fdINA, reg);
	//usleep(70);
	//temp.word = 0x1234;
	//data = temp.word / 10;
	//printf("0x%04x, %f\n", temp.word, data);

	temp2.lohi.hi = temp.lohi.lo;
	temp2.lohi.lo = temp.lohi.hi;
	double r = double(temp2.word);
	return r;
}

double INA::get_mA()
{
	return (get_Data(0x01) * 1.25);
}

double INA::get_V()
{
	return (get_Data(0x02) * 0.00125);
}

double INA::get_P()
{
	return (get_Data(0x03) * 10);
}
