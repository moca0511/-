#ifndef _PAST_TIME_H_
#define _PAST_TIME_H_

#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
extern struct timeval time_baseline;
extern time_t msTimeBaseline;
void setBaselineTime();
time_t getPastTime();

#endif
