#include <unistd.h>
#include "backgroundFunc.h"
#include "logFile/myLog.h"
#include "tcpServerClient/tcpStruct.h"
//#include "myFork/myFork.h"
void envPrint()
{
	const char *val = std::getenv(ENV_ROVER_DIR_PATH);
	if (val == nullptr)
	{ // invalid to assign nullptr to std::string
		printf("環境変数(PRO4_ROVER_DIR_PATH)が設定されていません\n");
		std::exit(0);
	}
	printf("rootPath:%s\n", std::getenv(ENV_ROVER_DIR_PATH));
}

//------------------------------
// main関数
//------------------------------
void main_rover();
std::string argv_string;
int main(int argc, char *argv[])
{
	// 引数をstringに格納
	for (int i = 1; i < argc; i++)
	{
		argv_string += argv[i];
		argv_string += " ";
	}

	envPrint();

	std::thread *pt[FUNC_NUM + 1];
	for (int i = 0; i < FUNC_NUM; i++)
	{
		pt[i] = new std::thread(backgroundFunc[i]);
	}
	pt[FUNC_NUM] = new std::thread(main_rover);

	printf("A\n");

	pt[FUNC__SERVER]->join(); //roverの終了まで待機

	printf("B\n");

	system("pkill str2str");
	end_flag_ = 0;

	printf("C\n");
	for (int i = 0; i < FUNC_NUM + 1; i++)
		if (i != FUNC__SERVER)
			pt[i]->join();
	printf("D\n");
}

void main_rover()
{
	printf("main_rover waiting...(%ds)\n", SERVER_LODING_TIME_MAIN);

	for (int i = 0; i < SERVER_LODING_TIME_MAIN; i++)
	{
		if (!end_flag_)
		{
			end_flag_ = 0;
			printf("\nrover.out end\n");
			return;
		}
		sleep(1);
	}
	printf("\nrover.out start\n");
/*
	for (int i = 0; i < 20; i++)
	{
		if (!end_flag_)
		{
			end_flag_ = 0;
			printf("\nrover.out end\n");
			return;
		}
		sleep(1);
	}
*/
	char temp[100];
	sprintf(temp, "sudo -E ./../rover/rover.out %s", argv_string.c_str());
	system(temp);
	printf("\nrover.out end\n");
}
