// ./a.out ccc eee  -h bbb -t bbb ddd -r a

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i, opt;

	opterr = 0; //getopt()のエラーメッセージを無効にする。

	while ((opt = getopt(argc, argv, "fgh:t:r:")) != -1)
	{
		//コマンドライン引数のオプションがなくなるまで繰り返す
		switch (opt)
		{
		case 'f':
			printf("-fがオプションとして渡されました\n");
			break;
		case 'g':
			printf("-gがオプションとして渡されました\n");
			break;
		case 'h':
			printf("-hがオプションとして渡されました\n");
			printf("引数optarg = %s\n", optarg);
			break;
		case 't':
			printf("-tがオプションとして渡されました\n");
			printf("引数optarg = %s\n", optarg);
			break;
		case 'r':
			printf("-rがオプションとして渡されました\n");
			printf("引数optarg = %s\n", optarg);
			break;

		default: /* '?' */
			//指定していないオプションが渡された場合
			printf("Usage: %s [-f] [-g] [-h argment] [-t argment] [-r argment] [-s] [-w] arg1 ...\n", argv[0]);
			break;
		}
	}

	printf("\n");

	//オプション以外の引数を出力する
	for (i = optind; i < argc; i++)
	{
		printf("arg(%d) = %s\n", i, argv[i]);
	}

	return 0;
}
