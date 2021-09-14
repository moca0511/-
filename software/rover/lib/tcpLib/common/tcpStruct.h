#ifndef _TCP_STRUCT_H_
#define _TCP_STRUCT_H_

#define MY_TCP_POT "3456"
#define ROVER_NAME "rover"
#include <string.h>

//kbhit()用
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#pragma pack(1) //構造体のメンバーの境界整列を制御

typedef struct
{
	int num1;
} msg0;

typedef struct
{
	char buf[3];
	double num2;
} msg1;

typedef struct
{
	char buf[2];
	char num3;
} msg2;

// モータの速度と時間
typedef struct
{
	int moterL;
	int moterR;
	int time; //	ms
} msg3;

// モータの速度と時間
typedef struct
{
	int moterOperation;
	int time; //	ms
} msg5;

//緯度経度
typedef struct
{
	double lon; //経度	x
	double lat; //緯度	y
} LON_LAT_POINT;

//目標地点の方向と距離
typedef struct
{
	double km;
	double direction;
} DISTINATION;

//  GPS総合
#define RECORD_END_LON_LAT 0
#define RECORD_END_DISTINATION 1
#define RECORD_RESULT 2
#define RECORD_NULL 3

//RTK_RESULT
#define RTK_RESULT__NULL 0
#define RTK_RESULT__SINGLE 1
#define RTK_RESULT__FIX  4
#define RTK_RESULT__FLOT 5

typedef struct
{
	LON_LAT_POINT start;
	LON_LAT_POINT now;
	LON_LAT_POINT end;
	DISTINATION distination;
	char measurementResultType; //rtk方の結果(fix, float)
	char sendType;				// 現在記録されているデータの種類
} msg4;

typedef union
{
	msg0 data0;
	msg1 data1;
	msg2 data2;
	msg3 data3;
	msg4 data4;
	msg5 data5;
} mainMsg;

// キュー
#define QUEUE__DO_NOW_AND_STACK_REMOVE 0
#define QUEUE__PUSH_AND_POP_ALLOWED 1
#define QUEUE__PUSH_AND_POP_DISALLOWED 2

#define CLIENT_USER_NAME_SIZE 32
#define CLIENT_BUF_SIZE 256
typedef struct
{
	char userName[CLIENT_USER_NAME_SIZE];
	char buf[CLIENT_BUF_SIZE];
	//int messageID;	//何番目のメッセージかを記録(希望)
	int dataType; //useDataの型を記録
	int queueAction;
	bool flgMonitor;
	mainMsg useData;
} tcpStruct;
#pragma pack() //構造体のメンバーの境界整列を制御(デフォルトに)

#define DISTINATION_STRUCT msg4

void init_tcpStruct_mainMsg(mainMsg *unionData);
void init_tcpStruct(tcpStruct *structData, const char userName[CLIENT_USER_NAME_SIZE]);
void init_struct_LON_LAT_POINT(LON_LAT_POINT *data);
void init_struct_distination(DISTINATION_STRUCT *data);

int kbhit(void);
#endif

	//http://www7b.biglobe.ne.jp/~robe/cpphtml/html03/cpp03014.html
