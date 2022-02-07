
#include <iostream>

//lsusb | grep U-Blox

int main()
{
	int ret = system("lsusb | grep U-Blox >/dev/null 2>&1");
	printf("%d\n", ret);
	if (ret == 0)
	{
		system("echo ${PRO4_ROVER_DIR_PATH}");
		//system("sh ${PRO4_ROVER_DIR_PATH}/src/main/f9p/addPath_str2str.sh ");
		system("sh ${PRO4_ROVER_DIR_PATH}/src/main/f9p/commdhome2.sh ");
		//system("sudo -E sh ${PRO4_ROVER_DIR_PATH}/src/main/f9p/addPath_str2str.sh >/dev/null 2>&1");
		//system("sudo -E sh ${PRO4_ROVER_DIR_PATH}/src/main/f9p/commdhome.sh >/dev/null 2>&1");
	}
	return EXIT_SUCCESS;
}
