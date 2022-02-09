

#include <time.h>

#include "myServer/tcpServer.h"

//struct timeval timeout = {5, 1000};
int i, ret;
//char tempBuf[512];

tcpStruct clientR, serverS;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("option : port\n");
		return 0;
	}

	ret = tcpServerSetup(argv[1]);
	if (ret != 0)
		return ret;

	while (1)
	{
		loopInit();
		switch (select(width, (fd_set *)&mask, NULL, NULL, NULL))
		//switch (select(width, (fd_set *)&mask, NULL, NULL, &timeout))
		{
		case -1: // エラーの場合
			printf("err:select\n");
			break;
		case 0: // タイムアウトの場合

			break;
		default: // 新規接続または接続済ソケットからの受信
			newConnect();

			// 接続済ソケットからのデータ受信かのチェック
			for (i = 0; i < CHILD_MAX; i++)
			{
				if (child[i] != -1)
				{
					// 接続済ソケット(child[i])からの受信の場合
					if (FD_ISSET(child[i], &mask))
					{
						// recv処理
						if (myRecv(i, &clientR, sizeof(tcpStruct)) != 0)
						{
							continue;
						}
						readFlg[i] = 1;
					}
				}
			}
			//受信したデータを加工
			for (i = 0; i < CHILD_MAX; i++)
			{
				if ((child[i] != -1) && (readFlg[i] != -1))
				{
					char *ptr;
					if ((ptr = strpbrk(clientR.buf, "\r\n")) != NULL)
					{
						*ptr = '\0';
					}
					printf("read[from:%s(id=%d)(dataType=%d)]:%s\n", clientR.userName,i,clientR.dataType, clientR.buf);
					for (int j = 0; j < CHILD_MAX; j++)
					{
						if ((child[i] != -1) && (i != j)) //受信した相手には送信しない
							sendFlg[j] = 1;
					}
					serverS = clientR;
					//strcpy(serverS.buf, clientR.buf);
					readFlg[i] = -1;
				}
			}
			//データの送信
			for (i = 0; i < CHILD_MAX; i++)
			{
				if ((child[i] != -1) && (sendFlg[i] != -1))
				{
					//send処理
					if (mySend(i, &serverS, sizeof(tcpStruct)) != 0)
					{
						continue;
					}

					printf("send[to:id=%d]\n", i);
					sendFlg[i] = -1;
				}
			}
			printf("\n");
			break; // case文のためのbreak
		}
	}
	return (EX_OK);
}
