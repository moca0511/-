#ifndef _GUIDE_SUPPORT_HPP_
#define _GUIDE_SUPPORT_HPP_

#include "guide/guide.h"

#define DOUBE_RAND_RANGE__LON_LAT 0.00001
#define DOUBE_RAND_RANGE__DIIRECTION 3

double getDoubleRand(double data, double range);
double getDoubleRand_LON_LAT(double data);
double getDoubleRand_direction(double data);

#endif
