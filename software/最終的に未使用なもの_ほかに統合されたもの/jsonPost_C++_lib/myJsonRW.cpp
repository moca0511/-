#include "jsonPost/myJsonRW.h"

//---------------------------
// writeクラス
//---------------------------
#define ADD_DATA_TEMP_STR_SIZE 25

static char tempTagData[ADD_DATA_TEMP_STR_SIZE];

std::string MY_WRITE_JSON::getWriteData()
{
	std::string tempTagData = writeData;
	if (addCount != 0)
	{
		int len = tempTagData.length();
		tempTagData.erase(len - 1);
	}
	return tempTagData;
}

void MY_WRITE_JSON::clear()
{
	flgWriteDataClear = false;
	addCount = 0;
	writeData.clear();
}

void MY_WRITE_JSON::addTagData(const char *tag, const int data)
{
	if (flgWriteDataClear)
		clear();
	addCount++;
	sprintf(tempTagData, "%s=%d&", tag, data);
	writeData += tempTagData;
}

void MY_WRITE_JSON::addTagData(const char *tag, const double data)
{
	if (flgWriteDataClear)
		clear();
	addCount++;
	sprintf(tempTagData, "%s=%f&", tag, data);
	writeData += tempTagData;
}

void MY_WRITE_JSON::addTagData(const char *tag, const bool data)
{
	if (flgWriteDataClear)
		clear();
	addCount++;
	sprintf(tempTagData, "%s=%d&", tag, data);
	writeData += tempTagData;
}

void MY_WRITE_JSON::addTagData(const char *tag, const char *data)
{
	if (flgWriteDataClear)
		clear();
	addCount++;
	writeData += tag;
	writeData += "=";
	writeData += data;
	writeData += "&";
}

//---------------------------
// readクラス
//---------------------------
#include <iostream>
#include <string>

bool strFlg = false;
static std::string tempData;
std::string MY_READ_JSON::getStr(const char *keyStr)
{
	std::string data = "";
	try
	{
		strFlg = true;
		data = readData[json::json_pointer(keyStr)];
	}
	catch (const std::logic_error &e)
	{
		strFlg = false;
		printf("readDataに%sは存在しない\n", keyStr);
	}
	return data;
};

double MY_READ_JSON::getTagData(const char tagStr[], const double num)
{
	tempData = getStr(tagStr);
	if (strFlg == false)
	{
		return 0;
	}
	double t = stod(tempData);
	return t;
}

int MY_READ_JSON::getTagData(const char tagStr[], const int num)
{
	tempData = getStr(tagStr);
	if (strFlg == false)
	{
		return 0;
	}
	int t = atoi(tempData.c_str());
	return t;
}

void MY_READ_JSON::getTagData(const char tagStr[], std::string *str)
{
	tempData = getStr(tagStr);
	if (strFlg == false)
	{
		*str = "\0";
	}
	*str = tempData;
}

//---------------------------
