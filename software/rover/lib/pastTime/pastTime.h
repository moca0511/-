#ifndef _PAST_TIME_H_
#define _PAST_TIME_H_

#include <ctime>

extern struct timeval time_baseline;
class myPastTime
{
private:
	time_t msTimeBaseline;

public:
	myPastTime() { setBaselineTime(); };
	void setBaselineTime();
	time_t getPastTime();
};

#endif
