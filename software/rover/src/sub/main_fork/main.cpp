#include <unistd.h>
#include "backgroundFunc.h"
#include "logFile/myLog.h"
#include "tcpServerClient/tcpStruct.h"
#include "myFork/myFork.h"
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
int main(int argc, char *argv[])
{
	int forkState;
	envPrint();
	MY_FORK myFork(false, FUNC_NUM + 1);
	if ((forkState = myFork.ifMyFork(backgroundFunc, FUNC_NUM, false)) <= FUNC_NUM)
	{
		if (forkState != 0) //ifMyFork()の引数を実行
			return 0;

		myFork.myWait(FUNC__ROVER); //main_roverの終了を検知するまでwait()
		// 自動で終了しないfuncを強制終了
		myFork.myKill(FUNC__INA_LOG);
		myFork.myKill(FUNC__SERVER);
		myFork.myKill(FUNC__TEMP_LOG);
		printf("wait kill\n");
		myFork.parent_wait_child();
		//親プロセスが全ての子プロセス終了後に行う場所
		sleep(1);
		printf("%s 全体終了, state:%d\n\n", __FILE__, forkState);
		return 0;
	}
	else
	{
		//子プロセスの移動ロボット制御部分
		printf("main_rover waiting...(%ds)\n", SERVER_LODING_TIME_MAIN);
		sleep(SERVER_LODING_TIME_MAIN);
		printf("\nrover.out start\n");
		//main_rover(argc, argv);
		char temp[100];
		char temp2[100] = "";
		if (argc >= 1)
		{
			for (int i = 1; i < argc; i++)
				sprintf(temp2, "%s %s", temp2, argv[i]);
		}
		sleep(20);
		//printf("-----%s\n", temp2);
		sprintf(temp, "sudo -E ./../rover/rover.out %s", temp2);
		system(temp);
		printf("\nrover.out end\n");
	}
}
