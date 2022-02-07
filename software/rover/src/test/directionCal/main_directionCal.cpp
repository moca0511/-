#include "pointCalculation/cal_direction.h"

#include <iostream> //printf

void doubleCal(double base, double now, double range)
{
	double temp;

	printf("base=%.2f, now=%.2f, range=%.2f\n", base, now, range);
	temp = getDifferenceDirection(base, now);
	printf("difference mod = %.2f\n", temp);
	printf("int range = %s\n", (ifDirectionRange(base, now, range) ? "true" : "false"));
	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc == 4)
	{
		double base = atof(argv[1]);
		double now = atof(argv[2]);
		double range = atof(argv[3]);

		doubleCal(base, now, range);
		return 0;
	}
	else if (argc == 1)
	{
		doubleCal(0, 90, 90);
		doubleCal(0, 90, 45);

		doubleCal(45, 90, 90);
		doubleCal(45, 90, 45);

		doubleCal(45, 0, 90);
		doubleCal(45, 0, 45);

		doubleCal(270, 0, 90);
		doubleCal(270, 0, 45);

		doubleCal(270, 1, 90);
		doubleCal(270, 1, 45);
		return 0;
	}
	printf("err option : base now range\n");

	return 1;
}
