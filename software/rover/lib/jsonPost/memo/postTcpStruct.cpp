#include "myPostJson/postTcpStruct.h"

#define ADD_DATA_TEMP_STR_SIZE 25

std::string MY_POST_JSON::getStr(const char *keyStr)
	{
		std::string data = "";
		try
		{
			data = readData[json::json_pointer(keyStr)];
		}
		catch (const std::logic_error &e)
		{
			printf("readDataに%sは存在しない\n", keyStr);
		}
		return data;
	};

void MY_POST_JSON::getSendPostData()
{
	sendData += "\0";
}

MY_POST_JSON::MY_POST_JSON(const char _url[])
{
	url = _url;
}

void MY_POST_JSON::addData(const char *tag, const int data)
{
	char temp[ADD_DATA_TEMP_STR_SIZE];
	sprintf(temp, "%s=%d&", tag, data);
	sendData += temp;
}

void MY_POST_JSON::addData(const char *tag, const double data)
{
	char temp[ADD_DATA_TEMP_STR_SIZE];
	sprintf(temp, "%s=%f&", tag, data);
	sendData += temp;
}

void MY_POST_JSON::addData(const char *tag, const bool data)
{
	char temp[ADD_DATA_TEMP_STR_SIZE];
	sprintf(temp, "%s=%d&", tag, data);
	sendData += temp;
}

void MY_POST_JSON::addData(const char *tag, const char *data)
{
	sendData += tag;
	sendData += "=";
	sendData += data;
	sendData += "&";
}

int MY_POST_JSON::requestPost()
{
	std::string temp;
	getSendPostData();
	temp = url_post_proc(url.c_str(), sendData.c_str());
	readData = json::parse(temp.c_str());
	return 0;
}

int MY_POST_JSON::getData(const char *tag, int *data)
{
	std::string temp = getStr(tag);
	*data = atoi(temp.c_str());
	return 0;
}

int MY_POST_JSON::getData(const char *tag, double *data)
{
	std::string temp = getStr(tag);
	double t = stod(temp);
	*data = t;
	return 0;
}

int MY_POST_JSON::getData(const char *tag, bool *data)
{
	std::string temp = getStr(tag);
	*data = atoi(temp.c_str());
	return 0;
}

int MY_POST_JSON::getData(const char *tag, std::string *data)
{
	*data = getStr(tag);
	return 0;
}

#ifdef TEST_JSON_TAG

double getRand(int min, int max)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> randomIntDistribution((double)min, (double)max);
	return randomIntDistribution(eng);
}

#endif

void makeSendData_execution(MY_POST_JSON *e, const LON_LAT sendData, const double dir, const double speed, const int errMsg)
{
	e->addData(JSON_SEND_TAG__LON, sendData.now.point.lon);
	e->addData(JSON_SEND_TAG__LAT, sendData.now.point.lat);

	e->addData(JSON_SEND_TAG__DIR, dir);
	e->addData(JSON_SEND_TAG__SPEED, speed);

	e->addData(JSON_SEND_TAG__RTK, sendData.rtkResult);
	e->addData(JSON_SEND_TAG__ERR, errMsg);

#ifdef TEST_JSON_TAG
	e->addData(JSON_TEST_SEND_TAG__MODE, (int)floor(getRand(0, 5)));

	e->addData(JSON_TEST_SEND_TAG__LON, getRand());
	e->addData(JSON_TEST_SEND_TAG__LAT, getRand());

	e->addData(JSON_TEST_SEND_TAG__DIR, (int)floor(getRand(0, 360)));
	e->addData(JSON_TEST_SEND_TAG__METER, getRand());

	e->addData(JSON_TEST_SEND_TAG__TIME_MOVE, (int)floor(getRand()));
	e->addData(JSON_TEST_SEND_TAG__TIME_POST, (int)floor(getRand()));
#endif
	e->getSendPostData();
}

void postExecution(const LON_LAT sendData, const double dir, const double speed, const int errMsg, tcpStruct *returnData)
{
	int mode;
	int i;
	MY_POST_JSON execution(SERVER_URL);

	makeSendData_execution(&execution, sendData, dir, speed, errMsg);

	execution.requestPost();

	execution.getData(JSON_READ_TAG__MODE, &mode);
	switch (mode)
	{
	case RETURN_POST_MODE__LON_LAT:
		execution.getData(JSON_READ_TAG__LON, &(returnData->useData.data4.point.lon));
		execution.getData(JSON_READ_TAG__LAT, &returnData->useData.data4.point.lat);
		returnData->dataType = 4;
		returnData->useData.data4.recodType = RECORD_END_LON_LAT;
		returnData->useData.data4.distination.m = 0;
		returnData->useData.data4.distination.direction = 0;
		break;
	case RETURN_POST_MODE__DIR_M:
		execution.getData(JSON_READ_TAG__DIR, &returnData->useData.data4.distination.direction);
		execution.getData(JSON_READ_TAG__METER, &returnData->useData.data4.distination.m);
		returnData->dataType = 4;
		returnData->useData.data4.recodType = RECORD_END_DISTINATION_ABSOLUTE;
		break;
	case RETURN_POST_MODE__MOTER_MP:
		returnData->useData.data5.moterOperation = M_OPERATION_MP;
		break;
	case RETURN_POST_MODE__MOTER_MB:
		returnData->useData.data5.moterOperation = M_OPERATION_MB;
		break;
	case RETURN_POST_MODE__MOTER_TL:
		returnData->useData.data5.moterOperation = M_OPERATION_TL;
		break;
	case RETURN_POST_MODE__MOTER_TR:
		returnData->useData.data5.moterOperation = M_OPERATION_TR;
		break;
	case RETURN_POST_MODE__MOTER_STOP:
		returnData->useData.data5.moterOperation = M_OPERATION_S;
		break;
	case RETURN_POST_MODE__OFF:
		printf("終了命令を受信%s(%s)\n", __FILE__, __func__);
		exit(1);
		break;
	default:
		break;
	}

	switch (mode)	//モータ制御の共通部分
	{
	case RETURN_POST_MODE__MOTER_MP:
	case RETURN_POST_MODE__MOTER_MB:
	case RETURN_POST_MODE__MOTER_TL:
	case RETURN_POST_MODE__MOTER_TR:
	case RETURN_POST_MODE__MOTER_STOP:
		returnData->dataType = 5;
		returnData->useData.data5.time = 100;
		break;	
	default:
		break;
	}
	return;
}
