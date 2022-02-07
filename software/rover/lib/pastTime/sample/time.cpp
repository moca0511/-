
#include "pastTime/pastTime.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

myPastTime mainPastTime;
int main()
{
	mainPastTime.setBaselineTime();

	sleep(2);
	cout << "milliseconds past time: 	" << mainPastTime.getPastTime() << endl;
	return EXIT_SUCCESS;
}
