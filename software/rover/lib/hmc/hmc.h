
#include <stdio.h>
#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>
/* 16bitの共用体 */
typedef union
{
	unsigned short int word;
	/* 8bit2個=16bitの構造体 */
	struct
	{
		unsigned char lo;
		unsigned char hi;
	} lohi;
} worder;

#define HMC_DEFAULT_VALUE__NO_USE	0
#define HMC_DEFAULT_VALUE__MEASURE	1	//測定
#define HMC_DEFAULT_VALUE__ARGUMENT 2	//引数

double getHMC();
void printHmc();
void initHMC(int num = HMC_DEFAULT_VALUE__MEASURE, double = 0);
void calibrationHmc(int num);
