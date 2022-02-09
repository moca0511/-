#include "myClient/tcpClient.h"

//linux用キー入力があるかの検知
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

//TCPサーバに接続
//引数：hostnm=ホスト名かIPアドレス、portnm=TCPサーバーポート番号
//戻り値：-1=エラー、0>=作成されたソケットを参照するディスクリプタ
int myTcpConnect(char *hostnm, char *portnm)
{
	char buf[512];
	struct sockaddr_in sa;
	struct in_addr addr;
	int sd, portno;
	struct hostent *host;
	struct servent *se;

	// アドレス情報をゼロクリア
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;

	// ここからIPアドレスの処理

	// hostnmがddd.ddd.ddd.ddd表記でなく名称指定の場合
	if (inet_pton(AF_INET, hostnm, &addr) == 0)
	{

		// ホスト名が名称としてホスト情報取得
		if ((host = gethostbyname2(hostnm, AF_INET)) == NULL)
		{
			(void)fprintf(stderr, "gethostbyname2():error\n");
			return (-1);
		}

		// hostent型構造体のhostのメンバーh_addr_listをキャストしてstruct in_addr型のaddrにコピー
		(void)memcpy(&addr, (struct in_addr *)*host->h_addr_list, sizeof(struct in_addr));
	}

	// struct in_addr型のaddrをドット区切りの 10 進数形式 "ddd.ddd.ddd.ddd" の IPv4 ネットワークアドレス(文字列)に変換。
	//返り値はbufへのポインタなのでIPアドレスがstderrに出力される
	(void)fprintf(stderr, "addr=%s\n", inet_ntop(AF_INET, &addr, buf, sizeof(buf)));

	sa.sin_addr = addr;

	// ここからポート番号の処理

	// ポート番号先頭が数値
	if (isdigit(portnm[0]))
	{

		// 数値化すると0以下ならエラー
		if ((portno = atoi(portnm)) <= 0)
		{
			(void)fprintf(stderr, "bad port no\n");
			return (-1);
		}

		// ネットワークバイトオーダーに変換してsa.sin_portに代入
		sa.sin_port = htons(portno);

		// ポート番号がサービス名で指定された
	}
	else
	{

		// 存在しないサービス名
		if ((se = getservbyname(portnm, "tcp")) == NULL)
		{
			(void)fprintf(stderr, "getservbyname():error\n");
			return (-1);

			// 存在した場合
		}
		else
		{

			// servent構造体のs_portはネットワークバイトオーダーのためそのまま代入可
			sa.sin_port = se->s_port;
		}
	}

	// (確認用)ntohsでネットワークバイトオーダーをホストバイトオーダーに変換してそのままstderrに出力させる
	(void)fprintf(stderr, "port=%d\n", ntohs(sa.sin_port));

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return (-1);
	}

	if (connect(sd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		perror("connect");
		(void)close(sd);
		return (-1);
	}

	/*
   ここで、ノンブロッキングに設定しています。
   val = 0でブロッキングモードに設定できます。
   ソケットの初期設定はブロッキングモードです。
 */
	int val = 1;
	ioctl(sd, FIONBIO, &val);

	return sd;
}

//サーバからデータ受信
//引数：サーバとのソケットのディスクリプタ、受信データ、受信データの大きさ
//戻り値：-1=切断、-2=err、0=変化なし、1=読み込み
int clientRecv(int sd, void *buf, int bufSize)
{
	int n;
	memset(buf, 0, bufSize);
	n = recv(sd, buf, bufSize, 0);
	if (n < 1)
	{
		if (n == 0)
		{
			printf("切断されました。\n");
			(void)close(sd);
			return -1;
		}

		if (errno == EAGAIN)
		{
			//printf("まだ来ない。\n");
			return 0;
		}
		else
		{
			perror("recv");
			(void)close(sd);
			return -2;
		}
	}
	return 1; //読み込み成功
}

//サーバにデータの送信
//引数：サーバとのソケットのディスクリプタ、送信データ、送信データの大きさ
//戻り値：0成功、１失敗
int clientSend(int sd, void *buf, int bufSize)
{
	ssize_t len;
	if ((len = send(sd, buf, bufSize, 0)) == -1)
	{
		perror("send");
		(void)close(sd);
		return 1;
	}
	return 0;
}
