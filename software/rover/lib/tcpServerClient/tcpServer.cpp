
#include "tcpServer.h"

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <cerrno>
#include <netdb.h>
#include <stdio.h>
#include <sysexits.h>

int flgExclusive_mainTcpServer = false; //mainTcpServerが一つだけであることを確認

// 引数1=trueなら、全てのmyPrint表示
// 引数2=trueなら、初めのクライアントが終了したらサーバも終了
// 引数3=0以上、サーバに接続したプロセスの内、(引数3)番目までのプログラムが終了したら、サーバも終了
int mainTcpServer(bool flgPrintMode, bool endMode, int endNum)
{
	my_tcp_server myTcpServer;
	myTcpServer.setPrintMode(flgPrintMode);

	if (flgExclusive_mainTcpServer)
	{
		myTcpServer.myPrint("myTcpServerはプログラム内で既に宣言されています。\n", true);
		return 1;
	}
	flgExclusive_mainTcpServer = true;

	int i, ret;

	tcpStruct clientR, serverS;
	char temp_str[100];
	ret = myTcpServer.tcpServerSetup(MY_TCP_POT);
	if (ret != 0)
		return ret;

	while (1)
	{
		myTcpServer.loopInit();
		switch (select(myTcpServer.width, (fd_set *)&myTcpServer.mask, NULL, NULL, NULL))
		{
		case -1: // エラーの場合
			printf("err:select\n");
			break;
		case 0: // タイムアウトの場合

			break;
		default: // 新規接続または接続済ソケットからの受信
			myTcpServer.newConnect();

			// 接続済ソケットからのデータ受信かのチェック
			for (i = 0; i < CHILD_MAX; i++)
			{
				if (myTcpServer.child[i] != -1)
				{
					// 接続済ソケット(myTcpServer.child[i])からの受信の場合
					if (FD_ISSET(myTcpServer.child[i], &myTcpServer.mask))
					{
						// recv処理
						if (myTcpServer.myRecv(i, &clientR, sizeof(tcpStruct)) != 0)
						{
							sprintf(temp_str, "server:disconnection(id=%d, endmode=%d)\n", i, endMode);
							myTcpServer.myPrint(temp_str);

							if ((endMode) && (i <= endNum))
							{
								myTcpServer.myPrint("server:Important Client disconnection\n", true);
								return 0;
							}
							continue;
						}
						myTcpServer.readFlg[i] = 1;
					}
				}
			}
			//受信したデータを加工
			for (i = 0; i < CHILD_MAX; i++)
			{
				if ((myTcpServer.child[i] != -1) && (myTcpServer.readFlg[i] != -1))
				{
					char *ptr, temp[CLIENT_BUF_SIZE];
					strcpy(temp, clientR.buf);
					if ((ptr = strpbrk(temp, "\r\n")) != NULL)
					{
						*ptr = '\0';
					}
					for (int j = 0; j < CHILD_MAX; j++)
					{
						if ((myTcpServer.child[i] != -1) && (i != j)) //受信した相手には送信しない
							myTcpServer.sendFlg[j] = 1;
					}
					serverS = clientR;
					myTcpServer.readFlg[i] = -1;
					sprintf(temp_str, "read[from:id=%d]%s\n", i, temp);
					myTcpServer.myPrint(temp_str);
				}
			}
			//データの送信
			int flg_print_n = false;
			for (i = 0; i < CHILD_MAX; i++)
			{
				if ((myTcpServer.child[i] != -1) && (myTcpServer.sendFlg[i] != -1))
				{
					//send処理
					if (myTcpServer.mySend(i, &serverS, sizeof(tcpStruct)) != 0)
					{
						continue;
					}
					sprintf(temp_str, "send[to : id=%d]\n", i);
					myTcpServer.myPrint(temp_str);
					myTcpServer.sendFlg[i] = -1;
					flg_print_n = true;
				}
			}
			if (flg_print_n)
			{
				myTcpServer.myPrint("\n");
				flg_print_n = false;
			}
			break; // case文のためのbreak
		}
		usleep(1000 * 1);
	}
	return (EX_OK);
}

//TCPサーバを用意
//引数：TCPサーバを配置するポート番号
//戻り値：-1=エラー、0>=作成されたソケットを参照するディスクリプタ
int my_tcp_server::server_sock(const char *portnm)
{
	struct sockaddr_in sa;
	struct in_addr addr;
	int sd, portno;
	struct servent *se;
	int yes;

	// アドレス情報をゼロクリア
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;

	// ホストバイト順序で表現された32ビット整数をネットワークバイト順序に変換
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	// ポート番号が数値で指定された場合

	// 指定されたポート番号の先頭が数値(文字列)かを判断
	if (isdigit(portnm[0]))
	{

		// 数値化すると0以下ならエラー
		if ((portno = atoi(portnm)) <= 0)
		{
			myPrint("bad port no\n", true);
			return (-1);
		}

		// ホストバイトオーダーをネットワークバイトオーダーに変換してsa.sin_portに代入
		sa.sin_port = htons(portno);

		// ポート番号がサービス名で指定された場合
	}
	else
	{
		// 存在しないサービス名
		if ((se = getservbyname(portnm, "tcp")) == NULL)
		{
			myPrint("getservbyname():error\n", true);
			return (-1);
			// サービス名が存在した場合
		}
		else
		{
			// servent構造体のs_portはネットワークバイトオーダーのためそのまま代入可
			sa.sin_port = se->s_port;
		}
	}

	// ntohsでネットワークバイトオーダーをホストバイトオーダーに変換してそのままstderrに出力させる
	char temp[11];
	sprintf(temp, "port=%d\n", ntohs(sa.sin_port));
	myPrint(temp);

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		myPrint("err:socket\n", true);
		return (-1);
	}

	//SO_REUSEADDRを設定すると、TIME_WAIT状態のポートが存在していてもbindができる
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	int t;
	// sdはソケット記述子。&saをstruct sockaddr_inからsockaddrへのポインタ型に変換
	if ((t = bind(sd, (struct sockaddr *)&sa, sizeof(sa))) == -1)
	{
		char temp[30];
		sprintf(temp, "err:bind(%d)\n", t);
		myPrint(temp, true);
		(void)close(sd);
		return (-1);
	}

	if (listen(sd, SOMAXCONN) == -1)
	{
		myPrint("err:listen\n", true);
		(void)close(sd);
		return (-1);
	}
	return sd;
}

//TCPサーバの準備を含めた通信関係の準備
//引数：TCPサーバのポート番号
//戻り値：EX_UNAVAILABLE=ソケットの準備失敗、0=正常終了
int my_tcp_server::tcpServerSetup(const char *port)
{
	// サーバソケット準備(socket→bind→listenを実行する関数)。本題では無いため本関数は割愛
	if ((soc = server_sock(port)) == -1)
	{
		char temp[30];
		sprintf(temp, "server_socket(%s):error\n", port);
		myPrint(temp, true);
		return (EX_UNAVAILABLE);
	}
	myPrint("ready for accept\n", true);

	// child[0]、child[1]、child[2]は接続中クライアントがいれば記述子が入る。いなければ-1。
	for (int i = 0; i < CHILD_MAX; i++)
		child[i] = -1;
	for (int i = 0; i < CHILD_MAX; i++)
		sendFlg[i] = -1;
	for (int i = 0; i < CHILD_MAX; i++)
		readFlg[i] = -1;

	return 0;
}

void my_tcp_server::loopInit()
{
	// select()でmaskが書き換えられるためループするごとに初期化
	FD_ZERO(&mask);

	// 接続待ちソケットを監視対象に追加
	FD_SET(soc, &mask);

	// select()の第一引数に使用。この時点では接続待ちソケット + 1
	width = soc + 1;

	// 監視対象の追加とselect()の第一引数の更新
	for (int i = 0; i < CHILD_MAX; i++)
	{
		if (child[i] != -1)
		{
			// 接続中のクライアントがいれば記述子を監視対象に追加
			FD_SET(child[i], &mask);

			// select()の第一引数を更新
			if (child[i] + 1 > width)
				width = child[i] + 1;
		}
	}

	//printf("%d\n", width);
}

//新しいクライアントの接続(上限以上は拒否)
void my_tcp_server::newConnect()
{
	int conectCount, acc;
	struct sockaddr_storage from;
	socklen_t len;
	// 新規接続の場合
	if (FD_ISSET(soc, &mask))
	{
		// accept処理
		len = (socklen_t)sizeof(from);
		if ((acc = accept(soc, (struct sockaddr *)&from, &len)) == -1)
		{
			if (errno != EINTR)
				myPrint("err:accept\n", true);
		}
		else
		{
			for (conectCount = 0; conectCount < CHILD_MAX; conectCount++)
			{
				if (child[conectCount] == -1)
				{
					child[conectCount] = acc;
					break;
				}
			}
			if (conectCount >= CHILD_MAX)
			{
				// すでに3接続がある場合は受け入れ拒否
				myPrint("child is full : cannot accept\n", true);
				(void)close(acc);
			}
		}
	}
}

//クライアントを切断するときに行う動作。
//引数：節残するクライアントの識別番号
void my_tcp_server::childClose(int fdId)
{
	// 切断された場合は以下の処理を実施
	(void)close(child[fdId]);
	child[fdId] = -1;
	sendFlg[fdId] = -1;
}

//クライアントにデータを送信
//引数：fdID=クライアントの識別番号、buf=送信データ、bufSize=送信データの大きさ
//戻り値：1=送信失敗、0=送信成功
int my_tcp_server::mySend(int fdId, void *buf, int bufsize)
{
	ssize_t len;
	if ((len = send(child[fdId], buf, bufsize, 0)) == -1)
	{
		myPrint("err:send\n");
		return 1;
	}
	return 0;
}

//クライアントからのデータ受信
//引数：fdID=クライアントの識別番号、buf=送信データ、bufSize=送信データの大きさ
//戻り値：1=受信失敗、0=受信成功
int my_tcp_server::myRecv(int fdId, void *buf, int bufsize)
{
	ssize_t len;
	int ret = 0;
	if ((len = recv(child[fdId], buf, bufsize, 0)) == -1)
	{
		myPrint("err:recv\n", true);
		ret = 1;
	}
	if (len == 0)
	{
		char temp[16];
		sprintf(temp, "server_recv:EOF(%d)\n", true);
		myPrint(temp);
		ret = 1;
	}

	if (ret == 1)
		childClose(fdId);
	return ret;
}

//自身のサーバ用ソケットの終了
void my_tcp_server::myClose()
{
	(void)close(soc);
}

void my_tcp_server::setPrintMode(bool num = true)
{
	printMode = num;
}

void my_tcp_server::myPrint(const char str[], bool important)
{
	if (printMode || important)
		printf("%s", str);
}
