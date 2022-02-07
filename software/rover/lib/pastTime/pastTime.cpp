#include "pastTime.h"

#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
struct timeval time_baseline
{
};
void myPastTime::setBaselineTime()
{
	gettimeofday(&time_baseline, nullptr);
	msTimeBaseline = (time_baseline.tv_sec * 1000) + (time_baseline.tv_usec / 1000);
}

time_t myPastTime::getPastTime()
{
	struct timeval time_now
	{
	};
	time_t msecs_time;

	gettimeofday(&time_now, nullptr);
	msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
	return (msecs_time - msTimeBaseline);
}
