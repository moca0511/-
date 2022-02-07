#ifndef _MY_QUEUE_H_
#define _MY_QUEUE_H_

#include "tcpServerClient/tcpStruct.h"

#include <queue>
class MY_QUEUE
{
private:
	std::queue<tcpStruct> queueServer;
	bool flgQueuePop = true;
public:
	void popTrue() { flgQueuePop = true; };
	void popFalse() { flgQueuePop = false; };
	void myPush(const tcpStruct data) { queueServer.push(data); };

	void myClear();
	bool ifPop();
	tcpStruct myPop();
	bool setPush(const tcpStruct data, bool flg);
};

#endif
