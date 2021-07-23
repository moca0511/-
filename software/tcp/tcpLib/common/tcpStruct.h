#ifndef _TCP_STRUCT_H_
#define _TCP_STRUCT_H_


#pragma pack(1) //構造体のメンバーの境界整列を制御
typedef struct 
{
	char userName[32];
	char buf[1024];
	int messageID;
} tcpStruct;
#pragma pack() //構造体のメンバーの境界整列を制御(デフォルトに)

#endif

//http://www7b.biglobe.ne.jp/~robe/cpphtml/html03/cpp03014.html
