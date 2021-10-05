
#include "pastTime/pastTime.h"
using std::cout;
using std::endl;


int main()
{
	setBaselineTime();

	sleep(2);
	cout << "milliseconds past time: 	" << getPastTime() << endl;
	return EXIT_SUCCESS;
}
