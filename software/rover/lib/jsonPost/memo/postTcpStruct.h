#ifndef _POST_TCP_STRUCT_H_
#define _POST_TCP_STRUCT_H_

#include <vector>
#include "tcpServerClient/tcpStruct.h"
#include "myPostJson/postJson.h"
#include "guide/guide.h"

#define TEST_JSON_TAG
#ifndef TEST_JSON_TAG
#define JOSN_READ_TAG__PARENT_TAG "/headers/"
#define JSON_SEND_TAG__PATERN_TAG ""
#else
#include <random>
#include <iomanip>
#define JOSN_READ_TAG__PARENT_TAG "/form/"
#define JSON_SEND_TAG__PATERN_TAG "send_"
#define MIN 10.0
#define MAX 100.0
double getRand(int min = MIN, int max = MAX);
#endif

#define SERVER_URL "https://httpbin.org/post"

//クライアント→サーバの送信データのタグ
#define JSON_SEND_TAG__LON "lon"
#define JSON_SEND_TAG__LAT "lat"
#define JSON_SEND_TAG__SPEED "speed"
#define JSON_SEND_TAG__DIR "dir"
#define JSON_SEND_TAG__RTK "rtk"
#define JSON_SEND_TAG__ERR "myErr"

//クライアント←サーバの送信データの作成用タグ
#define JSON_TEST_SEND_TAG__MODE (JSON_SEND_TAG__PATERN_TAG "mode")
#define JSON_TEST_SEND_TAG__LON (JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__LON)
#define JSON_TEST_SEND_TAG__LAT (JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__LAT)
#define JSON_TEST_SEND_TAG__DIR (JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__DIR)
#define JSON_TEST_SEND_TAG__METER (JSON_SEND_TAG__PATERN_TAG "meater")
#define JSON_TEST_SEND_TAG__TIME_MOVE (JSON_SEND_TAG__PATERN_TAG "timeM")
#define JSON_TEST_SEND_TAG__TIME_POST (JSON_SEND_TAG__PATERN_TAG "timeP")

//クライアント←サーバの送信データの読み取り用タグ
#define JSON_READ_TAG__MODE (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG "mode")
#define JSON_READ_TAG__LON (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__LON)
#define JSON_READ_TAG__LAT (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__LAT)
#define JSON_READ_TAG__DIR (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG JSON_SEND_TAG__DIR)
#define JSON_READ_TAG__METER (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG "meater")
#define JSON_READ_TAG__TIME_MOVE (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG "timeM")
#define JSON_READ_TAG__TIME_POST (JOSN_READ_TAG__PARENT_TAG JSON_SEND_TAG__PATERN_TAG "timeP")

//クライアント←サーバのモードのパターン
#define RETURN_POST_MODE__LON_LAT 1
#define RETURN_POST_MODE__DIR_M 2
#define RETURN_POST_MODE__MOTER_MP 3
#define RETURN_POST_MODE__MOTER_MB 4
#define RETURN_POST_MODE__MOTER_TL 5
#define RETURN_POST_MODE__MOTER_TR 6
#define RETURN_POST_MODE__MOTER_STOP 7
#define RETURN_POST_MODE__OFF 8


class MY_POST_JSON : public MY_POST_JSON_SIMPLE
{
private:
	int calling_tagListSize = 0;
	std::string url;
	std::string sendData;
	json readData;
	std::string getStr(const char *keyStr);
public:
	MY_POST_JSON(const char _url[]);
	void getSendPostData();

	void addData(const char *, const int);
	void addData(const char *, const double);
	void addData(const char *, const bool);
	void addData(const char *, const char *);

	int requestPost();

	int getData(const char *, int *);
	int getData(const char *, double *);
	int getData(const char *, bool *);
	int getData(const char *, std::string *);

	void myClear(){}; //sendData, readDataを初期化
	std::string getReadData() { return readData.dump(4); }
	void setSendData(const char *str) { sendData = str; };
	void printDump() { std::cout << getReadData() << "\n"; };
	std::string getSendData() { return sendData; }
};

void makeSendData_execution(MY_POST_JSON *e, const LON_LAT sendData, const double dir, const double speed, const int errMsg);
void postExecution(const LON_LAT sendData, const double dir, const double speed, const int errMsg, tcpStruct *returnData);

#endif
