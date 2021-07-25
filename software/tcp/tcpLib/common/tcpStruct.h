#ifndef _TCP_STRUCT_H_
#define _TCP_STRUCT_H_

#pragma pack(1) //構造体のメンバーの境界整列を制御

typedef struct
{
	int num1;
} msg1;

typedef struct
{
	char buf[3];
	double num2;
} msg2;

typedef struct
{
	char buf[2];
	char num3;
} msg3;

typedef union 
{
	msg1 data1;
	msg2 data2;
	msg3 data3;
} mainMsg;

typedef struct
{
	char userName[32];
	char buf[1024];
	int messageID;
	int dataType;
	mainMsg useData;
} tcpStruct;
#pragma pack() //構造体のメンバーの境界整列を制御(デフォルトに)

#endif

//http://www7b.biglobe.ne.jp/~robe/cpphtml/html03/cpp03014.html
