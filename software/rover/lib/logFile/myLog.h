/*
正常終了でしか、書き込まれないので注意
*/

#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <stdlib.h>

#define ENV_ROVER_DIR_PATH "PRO4_ROVER_DIR_PATH"
#define FILE_PATH_DEFF_LOG "./log/log.log"

#define FILE_PATH_NMEA_LOG "./log/rtk.nmea"

#define PATH_MODE__DEFF 0
#define PATH_MODE__NMEA 1

class myLogFile
{
private:
	std::ofstream writing_file;
	void write(const char str[]);
public:
	std::string filename;
	myLogFile();
	myLogFile(const char name[]);
	myLogFile(const int pathNum);
	~myLogFile();
	void fileInit();
	void print(const char str[]);
	void print(std::string str);
	void printSimple(const char str[]);
	std::string getDatetimeStr();
	const char *getRoverDirPath();

	void setAddRoverLogPath(const char path[]);
};

/*
memo:
writing_file.open(filename, std::ios::app); //std::ios::out=上書き、std::ios::app=追記

*/

#endif
