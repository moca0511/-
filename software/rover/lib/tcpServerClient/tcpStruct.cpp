#include "tcpStruct.h"
//kbhit()用
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

void init_tcpStruct_mainMsg(mainMsg *unionData)
{
	memset(unionData, 0, sizeof(mainMsg));
}

void init_tcpStruct(tcpStruct *structData,const char userName[CLIENT_USER_NAME_SIZE])
{
	memset(structData, 0, sizeof(tcpStruct));
	structData->flgMonitor = false;
	structData->flgTransfer_R2S = false;
	structData->queueAction = QUEUE__DO_NOW_AND_STACK_REMOVE;
	strcpy(structData->userName, userName);
	structData->buf[0] = '\0';
}

void init_struct_LON_LAT_POINT(LON_LAT_POINT *data)
{
	memset(data, 0, sizeof(LON_LAT_POINT));
}


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

int getSize_tcpStruct(){
	return sizeof(tcpStruct);
}
