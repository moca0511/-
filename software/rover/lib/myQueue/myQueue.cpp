#include "myQueue/myQueue.h"
#include <iostream>

void MY_QUEUE::myClear()
{
	while (!queueServer.empty())
		queueServer.pop();
	flgQueuePop = true;
}

bool MY_QUEUE::ifPop()
{
	if (flgQueuePop && (!queueServer.empty()))
		return true;
	return false;
}

tcpStruct MY_QUEUE::myPop()
{
	tcpStruct data = queueServer.front();
	queueServer.pop();
	return data;
}

bool MY_QUEUE::setPush(const tcpStruct data, bool flg)
{
	if (data.queueAction == QUEUE__PUSH_AND_POP_ALLOWED)
		popTrue();
	else if (data.queueAction == QUEUE__PUSH_AND_POP_DISALLOWED)
		popFalse();
	else if (data.queueAction == QUEUE__DO_NOW_AND_STACK_REMOVE)
	{
		myClear();
		flg = false;
	}
	else
	{
		printf("err pop flg\n");
		exit(1);
	}
	myPush(data);
	return flg;
}
