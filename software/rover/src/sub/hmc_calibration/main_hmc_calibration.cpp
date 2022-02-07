#include "hmc/hmc.h"
#include "pointCalculation/cal_direction.h"

//#define ARGC_FLG

int main(int argc, char const *argv[])
{
	roverMoter moter;
	moter.R.rotation(ROTATION_INVERSE_MAX);
	moter.L.rotation(ROTATION_REGULAR_MAX);
//sleep(30);
#ifdef ARGC_FLG
	if (argc != 2)
		return 0;
	calibrationHmc(atoi(argv[1]));
#else
	calibrationHmc(20);
#endif

	moter.R.rotation(M_PWM_STOP);
	moter.L.rotation(M_PWM_STOP);
	return 0;
}
