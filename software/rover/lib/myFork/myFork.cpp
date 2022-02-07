
#include "myFork.h"
#include <stdio.h>
#include <sys/types.h> // fork
#include <unistd.h>	   // fork
#include <sys/wait.h>  // wait, waitpid

MY_FORK::MY_FORK(const bool _flgPrint, const int _p_max)
{
	if (P_MAX <= _p_max)
	{
		printf("err(%s,%s):forkの数が多すぎます。\n", __FILE__, __func__);
		_exit(1);
	}
	p_max = _p_max;
	flgPrint = _flgPrint;
	start();
}

MY_FORK::~MY_FORK()
{
	for (pidElement = 0; pidElement < p_max; pidElement++)
		wait(&status);
}

void MY_FORK::start()
{
	for (pidElement = 0; (pidElement < p_max); pidElement++)
	{
		pid[pidElement] = fork();
		if (getPid(pidElement) == 0)
		{
			//printf("%d\n", pidElement);
			break;
		}
		else if (getPid(pidElement) < 0)
		{
			printf("fork err\n");
		}
	}
}

//親プロセスはすべての子プロセスの終了を待つ
void MY_FORK::parent_wait_child()
{
	forkPrint("check_child=");
	for (pidElement = 0; (pidElement < p_max); pidElement++)
	{
		if (flgPrint)
			printf("%d, ", getPid(pidElement));
	}
	forkPrint("\n");
	pid_t pid = 0;
	for (pidElement = 0; pidElement < p_max; pidElement++)
	{
		pid = wait(&status);
		if (flgPrint)
			printf("wain終了(%d)\n", pid);
	}
}

// MY_FORKの補助出力
void MY_FORK::forkPrint(const char str[])
{
	if (flgPrint)
		printf("%s", str);
}

//関数pfを使わずに複数の子プロセスを制御する時のif関数
//戻り値0=親プロセス(子プロセスが全て終了した)、0>なら要素%P_MAX番目の子プロセスである、0<エラー
int MY_FORK::ifMyFork(const bool waitOnPatent)
{
	if (pidElement == p_max) //親プロセスはすべての子プロセスの終了を待つ
	{
		if (waitOnPatent)
			parent_wait_child(); //親プロセスはすべての子プロセスの終了を待つ
	}
	else if (getPid(pidElement) == 0) //子プロセス
		return (pidElement % p_max) + 1;
	else
		return -1;
	return 0;
}

/*
forkの子プロセスにおいて、
pf_max番目までのプロセスでは関数pf[]を順番に実行する。
pf_max番目以降は特に何もしない。
[引数]
引数pf[]	：子プロセスの0~(pf_max-1)番目までで順に実行する関数配列
引数pf_max	：関数配列の要素数
引数waitOnPatent:親プロセスの時にwaitを実行する
[戻り値]
戻り値0=親プロセス(子プロセスが全て終了した)
戻り値0>なら要素%P_MAX番目の子プロセスである
戻り値0<エラー
*/
int MY_FORK::ifMyFork(void (*const pf[])(void), const int pf_max, const bool waitOnPatent)
{
	if (pidElement == p_max)
	{
		if (waitOnPatent)
			parent_wait_child(); //親プロセスはすべての子プロセスの終了を待つ
	}
	else if (getPid(pidElement) == 0)
	{ //子プロセス	//pidElementによって処理を変更
		const int nowElement = pidElement % p_max;
		if (nowElement < pf_max)
		{
			if (flgPrint)
				printf("childId(%d):fPoint=%d\n", nowElement, pf[nowElement]);
		}
		sprintf(temp_str, "child:%d, pid:%d, element=%d : ", pidElement, getpid(), nowElement);
		forkPrint(temp_str);
		if (nowElement < pf_max)
			pf[nowElement]();
		else
			forkPrint("no_func, ");
		return nowElement + 1;
	}
	else
	{
		forkPrint("child process");
		return -1;
	}
	return 0;
};

void MY_FORK::myWait(const int i)
{
	int status;
	if (flgPrint)
		printf("waitpid 開始(%d)\n", getPid(i));
	waitpid(getPid(i), &status, 0);
	if (flgPrint)
		printf("waitpid 完了(%d)\n", getPid(i));
}

void MY_FORK::myKill(const int i)
{
	kill(getPid(i), SIGTERM);
	if (flgPrint)
		printf("子プロセス強制終了(%d)\n", getPid(i));
}
