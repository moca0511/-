#include "moter/roverMoter.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
	roverMoter moter;

	moter.R.rotation(255);
	moter.L.rotation(0);
	sleep(3);
	return 0;
}
