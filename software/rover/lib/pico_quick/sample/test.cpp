#include "pico_quick/pico_quick.h"

void ttt()
{

	printf("outTest\n");
	system("gpio readall");
	//sleep(1);
	myGpio writePin(19, GPIO_MODE_OUT);
	system("gpio readall");
	sleep(1);
	for (int i = 0; i < 2; i++)
	{
		writePin.myWrite(1);
		usleep(1 * 1000);
		writePin.myWrite(0);
		usleep(1 * 1000);
		sleep(1);
	}
}

int main()
{

	pico_quick myPico;

	//Waitforinterrupt,repeat10times.
	while(1){
		myPico.getPico();
		for (int j = 0; j < 4; j++)
			printf("%d, ", myPico.getWallFlg(j));
		printf("\n");
		sleep(1);
}
	ttt();
	system("gpio readall");
	return (0);
}
