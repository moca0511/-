#ifndef _CAL_DIRECTION_H_
#define _CAL_DIRECTION_H_

#include "moter/roverMoter.h"
bool ifDirectionRange(double base, double now, double range);
double getDifferenceDirection(double base, double now);
int cal_moterPower_feedback(const double now, const double hope, MOTER_STRUCT *m, double main_range = 15.0);

#endif
