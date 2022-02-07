#include "myLog.h"

#include <iomanip>
#include <stdlib.h>

// デフォルトファイルにログを保存
myLogFile::myLogFile(const bool flgPrint)
{
	setAddRoverLogPath(FILE_PATH_DEFF_LOG);
	if (flgPrint)
		printf("log set %s\n", filename.c_str());
	myFileOpen();
}

// 引数1のファイルにログを保存
myLogFile::myLogFile(const char name[], const bool flgPrint)
{
	setAddRoverLogPath(name);
	if (flgPrint)
		printf("log set %s\n", filename.c_str());
	myFileOpen();
}

myLogFile::myLogFile(const int pathMode, const bool flgPrint)
{
	const char *addPath;
	switch (pathMode)
	{
	case PATH_MODE__DEFF:
		addPath = FILE_PATH_DEFF_LOG;
		break;
	case PATH_MODE__NMEA:
		addPath = FILE_PATH_NMEA_LOG;
		break;
	default:
		printf("err : %s : pathMode\n", __func__);
		exit(1);
		break;
	}
	setAddRoverLogPath(addPath);
	if (flgPrint)
		printf("log set %s\n", filename.c_str());
	myFileOpen();
}

myLogFile::~myLogFile()
{
	writing_file.close();
}

void myLogFile::myFileOpen()
{
	writing_file.open(filename, std::ios::app); //std::ios::out=上書き、std::ios::app=追記
	if (!writing_file)
	{
		printf("%sが開けませんでした\n", filename.c_str());
		if (writing_file.is_open())
		{
			printf("%sは既に開かれていました。\n");
		}
		else
		{
			return;
		}
	}
}

void myLogFile::setAddRoverLogPath(const char path[])
{
	std::string logPath;
	logPath = getRoverDirPath();
	logPath += path;
	filename = logPath;
}

void myLogFile::write(const char str[])
{
	if (!writing_file)
	{
		printf("failが開けていません\n");
		//ファイルが開けていない
		return;
	}
	writing_file << str;
	//writing_file.close();
	writing_file.flush();
}

void myLogFile::fileInit()
{
	writing_file.close();

	writing_file.open(filename, std::ios::out);
	writing_file << "\0";
	writing_file.close();
	myFileOpen();
}

std::string myLogFile::getDatetimeStr()
{
	time_t t = time(nullptr);
	const tm *localTime = localtime(&t);
	std::stringstream s;
	s << "20" << localTime->tm_year - 100;
	// setw(),setfill()で0詰め
	s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
	s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
	s << "_";
	s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
	s << std::setw(2) << std::setfill('0') << localTime->tm_min;
	s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
	// std::stringにして値を返す
	return s.str();
}

void myLogFile::print(const char str[])
{
	char tmp[1024];
	sprintf(tmp, "%s:%s", getDatetimeStr().c_str(), str);
	write(tmp);
}

void myLogFile::print(std::string str)
{
	char tmp[1024];
	sprintf(tmp, "%s:%s", getDatetimeStr().c_str(), str.c_str());
	write(tmp);
}

void myLogFile::printSimple(const char str[])
{
	write(str);
}

const char *myLogFile::getRoverDirPath()
{
	const char *val = std::getenv(ENV_ROVER_DIR_PATH);
	if (val == nullptr)
	{ // invalid to assign nullptr to std::string
		printf("err env\n");
		return "";
	}
	return val;
}
