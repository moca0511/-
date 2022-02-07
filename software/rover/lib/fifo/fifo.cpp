#include "fifo/fifo.h"
#include "tcpServerClient/consoleClient.h"
#include <sys/stat.h>

#include <sys/fcntl.h>
#include <errno.h>
#include <unistd.h>

//------------------------------
// 初期宣言
//------------------------------
int fifoFileCheck(const char); //fifo用ファイルが作成されていることを確認
int myRead(const int fd, void *buf, const int size);
void makeFifo(const char fileName[]);

#define FIFO_MASTER_TO_CLIENT "/tmp/fifo_A.fifo"
#define FIFO_CLIENT_TO_MASTER "/tmp/fifo_B.fifo"

#define TARGET_MODE_AUTO 5

//------------------------------

//------------------------------
// fifoの親と子で共通関数
//------------------------------
void makeFifo(const char fileName[])
{
	int ret = mkfifo(fileName, 0666);
	if (ret == -1)
	{
		//EEXIST=ファイルが既にある
		if (errno != EEXIST)
		{
			perror("mkfifo 失敗");
			exit(1);
		}
	}
}

//読み込んだデータはbufに代入
//戻り値1は読み込み成功
int myRead(const int fd, void *buf, const int size)
{
	int len = read(fd, buf, size);
	if (len < 0)
	{
		if (errno == EAGAIN)
		{
			//fprintf(stderr, "avoid blocking\n");
		}
		else
			perror("read");
		return 0;
	}
	else if (len == 0)
	{
		//printf("EOF\n"); // len == 0のときはパイプの中身を全て読んでいる状態。
		return 0;
	}
	return 1;
}

//------------------------------
// fifoの親
//------------------------------

FIFO_ROVER::FIFO_ROVER()
{
	makeFifo(FIFO_MASTER_TO_CLIENT);
	makeFifo(FIFO_CLIENT_TO_MASTER);

	//書き込みfd準備
	fdW = open(FIFO_MASTER_TO_CLIENT, O_RDWR | O_NONBLOCK);
	if (fdW == -1)
	{
		perror("open err fdW");
		exit(0);
	}

	//読み込みfd準備
	fdR = open(FIFO_CLIENT_TO_MASTER, O_RDONLY | O_NONBLOCK);
	if (fdR == -1)
	{
		perror("open err fdR");
		exit(0);
	}

	init_tcpStruct(&readData, "readMasterServer");
}

tcpStruct FIFO_ROVER::conv(const type_StoR dd)
{
	consoleTcpClient v;
	char temp[100];
	readData.queueAction = QUEUE__DO_NOW_AND_STACK_REMOVE;
	if (dd.mode == TARGET_MODE_AUTO)
	{
		readData.dataType = 4;
		sprintf(temp, "%d %f %f 0 0 0", RECORD_END_DISTINATION_RELATIVE, dd.data1, dd.data2);
		printf("get:%s\n", temp);
		v.char2tcpStruct(&readData, temp);
	}
	else
	{
		readData.dataType = 3;
		sprintf(temp, "%d %d 0 0", int(dd.data1), int(dd.data2));
		printf("get:%s\n", temp);
		v.char2tcpStruct(&readData, temp);
	}
	return readData;
}

int FIFO_ROVER::read_fifo_fromClient(tcpStruct *rData)
{
	//fifo読み込み
	int readSize = sizeof(type_StoR);
	void *buf = malloc(readSize);
	int ret = myRead(fdR, buf, readSize);
	if (ret == 0)
		return 0;

	//読み込んだデータを変換
	type_StoR *getData = (type_StoR *)buf;
	*rData = conv(*getData);
	return 1;
}

void FIFO_ROVER::write_fifo_toClient(const type_RtoS p)
{
	int len = write(fdW, &p, sizeof(type_RtoS));
}

//------------------------------
// fifoの子で共通関数
//------------------------------

FIFO_CLIENT::FIFO_CLIENT()
{
	makeFifo(FIFO_MASTER_TO_CLIENT);
	makeFifo(FIFO_CLIENT_TO_MASTER);

	//書き込みfd準備
	fdW = open(FIFO_CLIENT_TO_MASTER, O_RDWR | O_NONBLOCK);
	if (fdW == -1)
	{
		perror("open err fdW");
		exit(0);
	}

	//読み込みfd準備
	fdR = open(FIFO_MASTER_TO_CLIENT, O_RDONLY | O_NONBLOCK);
	if (fdR == -1)
	{
		perror("open err fdR");
		exit(0);
	}
}

void FIFO_CLIENT::write_fifo_toMaster(const type_StoR p)
{
	int len = write(fdW, &p, sizeof(type_StoR));
}

int FIFO_CLIENT::read_fifo_fromMaster(type_RtoS *p)
{ //fifo読み込み
	int readSize = sizeof(type_RtoS);
	void *buf = malloc(readSize);
	int ret = myRead(fdR, buf, readSize);
	if (ret == 0)
		return 0;
	type_RtoS *d = (type_RtoS *)buf;
	*p = *d;
	return 1;
}
