#ifndef _TCP_STRUCT_H_
#define _TCP_STRUCT_H_

#define ADDR__F9P_RTK "127.0.0.1"
#define TCP_PORT__F9P_RTK "2103"

#define ADDR__MY_TCP_SERVER "127.0.0.1"
#define MY_TCP_POT "3456"

#define ROVER_NAME "rover"

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
	double m;
	double direction;
} DISTINATION;

//  GPS総合
#define RECORD_NULL 0
#define RECORD_END_LON_LAT 1
#define RECORD_END_DISTINATION_ABSOLUTE 2 //絶対値
#define RECORD_END_DISTINATION_RELATIVE 4 //相対値
#define RECORD_RESULT 3

//RTK_RESULT
typedef struct
{
	LON_LAT_POINT point;
	DISTINATION distination;
	char recodType; // 現在記録されているデータの種類
} msg4;

typedef struct
{
	LON_LAT_POINT offsetSupport;
} msg6;

typedef union
{
	msg0 data0;
	msg1 data1;
	msg2 data2;
	msg3 data3;
	msg4 data4;
	msg5 data5;
	msg6 data6;
} mainMsg;

// キュー
#define QUEUE__DO_NOW_AND_STACK_REMOVE 0
#define QUEUE__PUSH_AND_POP_ALLOWED 1
#define QUEUE__PUSH_AND_POP_DISALLOWED 2

#define CLIENT_USER_NAME_SIZE 10
#define CLIENT_BUF_SIZE 512
typedef struct
{
	char userName[CLIENT_USER_NAME_SIZE];
	char bufName[CLIENT_USER_NAME_SIZE];
	char buf[CLIENT_BUF_SIZE];
	//int messageID;	//何番目のメッセージかを記録(希望)
	int dataType; //useDataの型を記録
	int queueAction;
	bool flgMonitor;
	bool flgTransfer_R2S;
	mainMsg useData;
} tcpStruct;
#pragma pack() //構造体のメンバーの境界整列を制御(デフォルトに)

void init_tcpStruct_mainMsg(mainMsg *unionData);
void init_tcpStruct(tcpStruct *structData, const char userName[CLIENT_USER_NAME_SIZE]);
void init_struct_LON_LAT_POINT(LON_LAT_POINT *data);
int getSize_tcpStruct();
int kbhit(void);
#endif

//http://www7b.biglobe.ne.jp/~robe/cpphtml/html03/cpp03014.html
