#include "hmc/hmc.h"
#include "pointCalculation/cal_direction.h"
//g++ hmc.cpp -lwiringPi && ./a.out

MOTER_STRUCT m;
int mOp;
double nowDir;

int main(int argc, char const *argv[])
{
	roverMoter moter;
	if (argc == 2)
	{
		moter.R.rotation(1);
		moter.L.rotation(255);
		//sleep(30);
		calibrationHmc(atoi(argv[1]));

		moter.R.rotation(M_PWM_STOP);
		moter.L.rotation(M_PWM_STOP);
		return 0;
	}

	//initHMC(HMC_DEFAULT_VALUE__ARGUMENT, 84.7);
	initHMC();
	while (1)
	{
		nowDir = getHMC();
		mOp = cal_moterPower_feedback(nowDir, 0, &m, 45);
		if (m.moterL == ROTATION_REGULAR_MAX && m.moterR == ROTATION_REGULAR_MAX)
		{
			m.moterL = M_PWM_STOP;
			m.moterR = M_PWM_STOP;
		}
		moter.R.rotation(m.moterR);
		moter.L.rotation(m.moterL);
		printf("dir=%5.1f,mOp=%d, m(%3d, %3d), t=%4d\n", nowDir, mOp, m.moterL, m.moterR, m.time);
		usleep(m.time*1000);
	}
	return 0;
}
