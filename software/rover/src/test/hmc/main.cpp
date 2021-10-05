#include "hmc6352/hmc.h"
//g++ hmc.cpp -lwiringPi && ./a.out


int main(int argc, char const *argv[])
{

	if(argc==2){
		calibrationHmc();
		return 0;
	}
	initHMC();
	while (1)
	{
		printHmc();
		sleep(1);
	}
	return 0;
}
