#include "cal_direction.h"
#include <math.h>
#include <cstdlib> // std::atoi, std::strtol

/*
baseを基準とした時±rangeの範囲内にnowがある
*/
bool ifDirectionRange(double base, double now, double range)
{
	if (abs(differenceDirection(base, now)) <= range)
		return true;
	return false;
}

/*
baseを基準とした時のnowとの差を戻り値で返す。
戻り値の範囲は±180
*/
double differenceDirection(double base, double now)
{
	double temp;
	temp = fmod(now - base + 360.0, 360.0);
	if (temp > 180.0)
		temp -= 360.0;

	return temp;
}
