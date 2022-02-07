// g++ thread.cpp -pthread ; ./a.out
#include <thread>
#include <cstdio>
#include <cstdint>

uint32_t end_flag_;
uint32_t msg_;

void ThreadA()
{
	int32_t input = 0;
	while (end_flag_)
	{
		printf("数字を入力してください\n");
		printf("0...スレッドBにメッセージを送信します\n");
		printf("1...プロセスを終了します\n");
		scanf("%d", &input);
		switch (input)
		{
		case 0:
			msg_ = 1;
			break;
		case 1:
			end_flag_ = 0;
			break;
		default:
			printf("０か１を入力してください\n");
			break;
		}
	}
	printf("スレッドA終了\n");
}

void ThreadB()
{
	while (end_flag_)
	{
		if (msg_)
		{
			printf("スレッドAからメッセージを受信しました\n");
			msg_ = 0;
		}
	}
	printf("スレッドB終了\n");
}

void ThreadC()
{
	printf("スレッドC終了\n");
}

void ThreadD()
{
	system("str2str");

	printf("スレッドC終了\n");
}

#define FUNC_NUM 4
void (*const backgroundFunc[FUNC_NUM])(void) = {
	ThreadA,
	ThreadB,
	ThreadC,
	ThreadD,
};

int main()
{
	msg_ = 0;
	end_flag_ = 1;
	std::thread *pt[FUNC_NUM];

	for (int i = 0; i < FUNC_NUM; i++)
	{
		pt[i] = new std::thread(backgroundFunc[i]);
	}

	printf("b\n");
	for (int i = 0; i < FUNC_NUM; i++){
		if (i == 3)
			system("pkill str2str");
		pt[i]->join();
	}

	return 0;
}
