#include "guide/guideSupport.h"
#include <random>

double getDoubleRand(double data, double range)
{
	double myRandData;
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distr(data - range, data + range);

	myRandData = distr(eng);

	return myRandData;
}

double getDoubleRand_LON_LAT(double data)
{
	return getDoubleRand(data, DOUBE_RAND_RANGE__LON_LAT);
}

double getDoubleRand_direction(double data)
{
	double temp = getDoubleRand(data, DOUBE_RAND_RANGE__DIIRECTION) * 10;
	temp = floor(temp);
	return (temp / 10);
}
