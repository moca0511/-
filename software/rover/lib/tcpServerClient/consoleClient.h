#ifndef CONSOLE_CLIENT_H
#define CONSOLE_CLIENT_H
#include "tcpClient.h"
#include <vector>
#include <string.h>
#include <sstream>

class consoleTcpClient
{
	std::vector<std::string> v;
	int vectorSize;

	void str2vector(const char data[]);
	void vectorClear();
	int vCount() { return vectorSize; };
	std::string pop(int num);
public:
	void printConsoleSupport(int);
	void char2tcpStruct(tcpStruct *data, const char str_in[]);
	std::string tcpStruct2string(tcpStruct *data);
};
#endif
