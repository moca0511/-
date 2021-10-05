
#include "myServer/tcpServer.h"

int child[CHILD_MAX];

int soc;
fd_set mask;
int width;
int sendFlg[CHILD_MAX];
int readFlg[CHILD_MAX];

//TCPサーバを用意
//引数：TCPサーバを配置するポート番号
//戻り値：-1=エラー、0>=作成されたソケットを参照するディスクリプタ
int server_sock(const char *portnm)
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
			(void)fprintf(stderr, "bad port no\n");
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
			(void)fprintf(stderr, "getservbyname():error\n");
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
	(void)fprintf(stderr, "port=%d\n", ntohs(sa.sin_port));

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("err:socket");
		return (-1);
	}

	//SO_REUSEADDRを設定すると、TIME_WAIT状態のポートが存在していてもbindができる
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	// sdはソケット記述子。&saをstruct sockaddr_inからsockaddrへのポインタ型に変換
	if (bind(sd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		printf("err:bind\n");
		(void)close(sd);
		return (-1);
	}

	if (listen(sd, SOMAXCONN) == -1)
	{
		printf("err:listen\n");
		(void)close(sd);
		return (-1);
	}
	return sd;
}

//TCPサーバの準備を含めた通信関係の準備
//引数：TCPサーバのポート番号
//戻り値：EX_UNAVAILABLE=ソケットの準備失敗、0=正常終了
int tcpServerSetup(const char *port)
{
	// サーバソケット準備(socket→bind→listenを実行する関数)。本題では無いため本関数は割愛
	if ((soc = server_sock(port)) == -1)
	{
		(void)fprintf(stderr, "server_socket(%s):error\n", port);
		return (EX_UNAVAILABLE);
	}
	(void)fprintf(stderr, "ready for accept\n");

	// child[0]、child[1]、child[2]は接続中クライアントがいれば記述子が入る。いなければ-1。
	for (int i = 0; i < CHILD_MAX; i++)
	{
		child[i] = -1;
	}

	for (int i = 0; i < CHILD_MAX; i++)
	{
		sendFlg[i] = -1;
	}
	for (int i = 0; i < CHILD_MAX; i++)
	{
		readFlg[i] = -1;
	}

	return 0;
}

void loopInit()
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
			{
				width = child[i] + 1;
			}
		}
	}

	//printf("%d\n", width);
}

//新しいクライアントの接続(上限以上は拒否)
void newConnect()
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
			{
				printf("err:accept");
			}
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
				(void)fprintf(stderr, "child is full : cannot accept\n");
				(void)close(acc);
			}
		}
	}
}

//クライアントを切断するときに行う動作。
//引数：節残するクライアントの識別番号
void childClose(int fdId)
{
	// 切断された場合は以下の処理を実施
	(void)close(child[fdId]);
	child[fdId] = -1;
	sendFlg[fdId] = -1;
}

//クライアントにデータを送信
//引数：fdID=クライアントの識別番号、buf=送信データ、bufSize=送信データの大きさ
//戻り値：1=送信失敗、0=送信成功
int mySend(int fdId, void *buf, int bufsize)
{
	ssize_t len;
	if ((len = send(child[fdId], buf, bufsize, 0)) == -1)
	{
		printf("err:send\n");

		return 1;
	}
	return 0;
}

//クライアントからのデータ受信
//引数：fdID=クライアントの識別番号、buf=送信データ、bufSize=送信データの大きさ
//戻り値：1=受信失敗、0=受信成功
int myRecv(int fdId, void *buf, int bufsize)
{
	ssize_t len;
	int ret = 0;
	if ((len = recv(child[fdId], buf, bufsize, 0)) == -1)
	{
		printf("err:recv\n");
		ret = 1;
	}
	if (len == 0)
	{
		(void)fprintf(stderr, "recv:EOF\n");
		ret = 1;
	}

	if (ret == 1)
	{
		childClose(fdId);
	}
	return ret;
}

//自身のサーバ用ソケットの終了
void myClose()
{
	(void)close(soc);
}
