#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <ulimit.h>
#include <stdlib.h>
#include <unistd.h>
#define FIFO "/tmp/icm.fifo"

class myFIFO_ICM
{
private:
	int fd;
	char buf[256];
	int len;

public:
	myFIFO_ICM();
	~myFIFO_ICM();
	int myRead(double *rData);
};

int main()
{
	myFIFO_ICM icm;
	double d;
	while (1)
	{
		if (icm.myRead(&d) == 1)
		{
			printf("%4.2lf\n", d);
		}
		sleep(1);
	}
	return 0;
}

int myFIFO_ICM::myRead(double *rData)
{
	int count;
	char tmp[256];
	len = read(fd, buf, sizeof(buf) - 1);
	if (len == 0)
	{
		//break;
		return 0;
	}
	else
	{
		buf[len] = '\0';
		int i;
		for (i = len - 2; i >= 0; i--) //len-1は最終データの[,]を指してしまうので不可
		{
			if (buf[i] == ',')
				break;
		}
		count = 0;
		for (int j = i + 1; j < len; j++)
		{
			tmp[count] = buf[j];
			count++;
		}
		len = count - 1;

		tmp[len] = '\0';
		*rData = std::strtod(tmp, NULL);
		return 1;
	}
}

myFIFO_ICM::myFIFO_ICM()
{
	/*
	int ret = mkfifo(FIFO, 0666);
	if (ret == -1)
	{
		perror("mkfifo");
	}
	*/
	if ((fd = open(FIFO, O_RDONLY | O_NONBLOCK)) == -1)
	{
		perror("open");
		exit(-1);
	}
	read(fd, buf, sizeof(buf) - 1); //残りバッファを読み込んで置く
}

myFIFO_ICM::~myFIFO_ICM()
{
	close(fd);
	//system("rm -f " FIFO);
}
