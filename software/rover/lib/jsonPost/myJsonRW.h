#ifndef _MY_JSON_READ_WRITE_H_
#define _MY_JSON_READ_WRITE_H_

//---------------------------
// writeクラス
//---------------------------

#include <string>

class MY_WRITE_JSON
{
private:
	std::string writeData;
	int addCount = 0;
	bool flgWriteDataClear=false;

public:
	std::string getWriteData();
	void clear();
	void setClearFlg_writeData() { flgWriteDataClear = true; };

	void addTagData(const char *, const int);
	void addTagData(const char *, const double);
	void addTagData(const char *, const bool);
	void addTagData(const char *, const char *);
};

//---------------------------
// readクラス
//---------------------------

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define MY_INT 0
#define MY_DOUBLE 1.1

class MY_READ_JSON
{
private:
	json readData;
	std::string getStr(const char *keyStr);

public:
	void setReadData(json s) { readData = s; };
	json getReadData() { return readData; };

	double getTagData(const char tagStr[], const double num);
	int getTagData(const char tagStr[], const int num = 0);
	void getTagData(const char tagStr[], std::string *str);
};

#endif
