#include "tcp.h"

int main(int argc, char *argv[])
{

	//引数の数チェック
	if (argc != 3 & argc != 4)
	{
		fprintf(stderr, "argument count mismatch error.\n");
		fprintf(stderr, "hostname port [mode]\n");
		exit(EXIT_FAILURE);
	}

	//クラスのモード確認
	tcpClient myClient(MODE_READ_RESULT);
	if (argc == 4)
	{
		if (strcmp(argv[3], "-s") == 0)
		{
			printf("set mode : speed\n");
			myClient.setMode(MODE_READ_SPEED);
		}
		else if (strcmp(argv[3], "-r") == 0)
		{
			printf("set mode : result\n");
			myClient.setMode(MODE_READ_RESULT);
		}
		else
		{
			printf("mode err\n");
			return 0;
		}
	}

	//tcpクラスの接続
	char ipAdd[256];
	myClient.hostname2ip(argv[1], ipAdd); //ホスト名からIPアドレス取得
	myClient.myConnect(ipAdd, argv[2]);	  //IPアドレスとポート番号を参考にTCP接続
	//myClient.access(argv[1], argv[2]);

	while (1)
	{

		myClient.read(); //データ受信
		myClient.scan(); //受信データを読み込みやすい形に変更
		//myClient.print();	//受信データの表示
		myClient.print_strtokc(); //受信データから必要なデータを取り出す

		if (myClient.nowMode == MODE_READ_SPEED)
			printf("%f, %f\n", myClient.angele, myClient.speed);
		else
			printf("%f, %f, %f, %f\n", myClient.lat, myClient.lon, myClient.hei, myClient.ratio);
	}

	return EXIT_SUCCESS;
}
