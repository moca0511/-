//https://kido0617.github.io/others/2013-01-06-c-fork/
#include <stdio.h>
#include "myFork/myFork.h"
#include <stdlib.h>
#include <unistd.h>

void func_A(int n, bool a);
void func_B();
void func_C(int n, bool a);
void func_D();
void func_E();
void func_F();
void func_G();

#define FORK_FUNC_NUM 2

// 複数プロセスの実行
// 変数p[]の子プロセスを実行
// childForkCountがFORK_FUNC_NUMより多い時は、point Aを実行する
void (*const p[FORK_FUNC_NUM])(void) = {func_B, func_D};
int main2()
{
	int forkState;
	const int childForkCount = 3;
	MY_FORK myFork2(true, childForkCount);
	if ((forkState = myFork2.ifMyFork(p, FORK_FUNC_NUM)) > myFork2.getP_Max() - 1)
	{
		//point A
		//FORK_FUNC_NUM番目の子プロセスである
		printf("奇数です");
		printf("\n");
		exit(0);
	}
	else
	{
		if (forkState != 0) //ifMyFork()の引数を実行
		{
			printf("fork end\n");
			exit(0);
		}
		printf("p_max=%d\n", myFork2.getP_Max());
		printf("state:%d\n", forkState); //親プロセスが全ての子プロセス終了後に行う場所
		return 0;
	}
	return 0;
}

// 複数プロセスの実行
// killなどを実装
#define FORK_FUNC_NUM_2 3
void (*const p2[FORK_FUNC_NUM_2])(void) = {func_E, func_F, func_G};
int main3()
{
	int forkState;
	const int childForkCount = FORK_FUNC_NUM_2 + 1;
	MY_FORK myFork2(true, childForkCount);
	if ((forkState = myFork2.ifMyFork(p2, FORK_FUNC_NUM_2, false)) > myFork2.getP_Max() - 1)
	{
		//point A
		//FORK_FUNC_NUM番目の子プロセスである
		printf("奇数です");
		printf("\n");
		exit(0);
	}
	else
	{
		if (forkState != 0) //ifMyFork()の引数を実行
		{
			printf("fork end\n");
			exit(0);
		}
		printf("親プロセス\n");
		myFork2.myWait(3);
		myFork2.myWait(2);
		myFork2.myKill(0);
		sleep(5);
		printf("子プロセス強制終了\n");
		myFork2.myKill(1);
		myFork2.parent_wait_child();
		printf("p_max=%d\n", myFork2.getP_Max());
		printf("state:%d\n", forkState); //親プロセスが全ての子プロセス終了後に行う場所

		printf("\n");
		printf("%d\n", func_F);
		printf("%d\n", p2[1]);
		return 0;
	}
	return 0;
}

int main()
{
	main2();
	printf("\nnext test\n\n");
	main3();
}

//------------------------------
//補助関数
//------------------------------

void func_A(int n, bool a)
{
	printf("by func_A, n^3 = %d, %d\n", n * n * n, a);
}

void func_B()
{
	func_A(11, true);
}

void func_C(int n, bool a)
{
	printf("by func_C, n^2 = %d, %d\n", n * n, a);
}

void func_D()
{
	func_C(11, false);
}

void func_E()
{
	while (1)
	{
		printf("a\n");
		sleep(1);
	}
}

void func_F()
{
	while (1)
	{
		printf("b\n");
		sleep(1);
	}
}

void func_G()
{
	for (int i = 0; i < 3; i++)
	{
		printf("c\n");
		sleep(1);
	}
}
