#include "cal_direction.h"
#include <math.h>
#include <cstdlib> // std::atoi, std::strtol

/*
baseを基準とした時±rangeの範囲内にnowがある
*/
bool ifDirectionRange(double base, double now, double range)
{
	if (abs(getDifferenceDirection(base, now)) <= range)
		return true;
	return false;
}

/*
baseを基準とした時のnowとの差を戻り値で返す。
戻り値の範囲は±180
*/
double getDifferenceDirection(double base, double now)
{
	double temp;
	temp = fmod(now - base + 360.0, 360.0);
	if (temp > 180.0)
		temp -= 360.0;

	return temp;
}

#define M_PWM_MIN (M_PWM_STOP / 4) * 3

//戻り値：mOp
int cal_moterPower_feedback(const double now, const double hope, MOTER_STRUCT *m, double main_range)
{
	int mOperation;
	if (ifDirectionRange(hope, now, main_range))
	{
		if (ifDirectionRange(hope, now, 7.5))
		{
			mOperation = M_OPERATION_MP;
			operation2moter(mOperation, m, 100);
		}
		else
		{
			int p = (int)(abs(getDifferenceDirection(hope, now)) * 7.5);
			if (p > M_PWM_STOP)
				p = M_PWM_STOP;
			else if (M_PWM_MIN < p)
				p = M_PWM_MIN;
			if (ifDirectionRange(hope + 90, now, 90.0))
			{
				m->moterL = ROTATION_REGULAR_MAX;
				m->moterR = ROTATION_REGULAR_MAX - p;
				/*
				m->moterL = ROTATION_REGULAR_MAX;
				m->moterR = ROTATION_INVERSE_MAX;
				mOperation = M_OPERATION_TR;
				*/
			}
			else
			{
				m->moterL = ROTATION_REGULAR_MAX - p;
				m->moterR = ROTATION_REGULAR_MAX;
				/*
				m->moterL = ROTATION_INVERSE_MAX;
				m->moterR = ROTATION_REGULAR_MAX;
				mOperation = M_OPERATION_TL;
				*/
			}
			m->time = 100;
			mOperation = moter2operation(m->moterL, m->moterR);
		}
	}
	else
	{
		if (ifDirectionRange(hope + 90, now, 90.0))
			mOperation = M_OPERATION_TR;
		else
			mOperation = M_OPERATION_TL;
		operation2moter(mOperation, m, 100);
	}
	return mOperation;
}
