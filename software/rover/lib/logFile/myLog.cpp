#include "myLog.h"

myLogFile::myLogFile(const char name[])
{
	filename = name;
}

myLogFile::myLogFile()
{
	printf("log set\n");
	setAddRoverLogPath(FILE_PATH_DEFF_LOG);
	printf("%s\n", filename.c_str());
}

myLogFile::myLogFile(const int pathMode)
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
	printf("log set\n");
	setAddRoverLogPath(addPath);
	printf("%s\n", filename.c_str());
}

myLogFile::~myLogFile()
{
	writing_file.close();
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
	writing_file.open(filename, std::ios::app); //std::ios::out=上書き、std::ios::app=追記
	writing_file << str;
	writing_file.close();
}

void myLogFile::fileInit()
{
	//writing_file.close();

	writing_file.open(filename, std::ios::out);
	writing_file << "\0";
	writing_file.close();
	//writing_file.open(filename, std::ios::app); //std::ios::out=上書き、std::ios::app=追記
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
	std::string print = getDatetimeStr() + " : " + str;
	write(print.c_str());
}

void myLogFile::print(std::string str)
{
	std::string print = getDatetimeStr() + " : " + str;
	write(print.c_str());
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
