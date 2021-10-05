
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

double getHMC();
void printHmc();
void initHMC();
void calibrationHmc();
