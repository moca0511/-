/*
正常終了でしか、書き込まれないので注意
*/

#ifndef _MY_LOG_H_
#define _MY_LOG_H_

#include <sstream>
#include <fstream>

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
	void myFileOpen();

public:
	std::string filename;
	myLogFile(const bool flgPrint = true);
	myLogFile(const char name[], const bool flgPrint = true);
	myLogFile(const int pathNum, const bool flgPrint = true);
	~myLogFile();
	void fileInit();
	void print(const char str[]);
	void print(std::string str);
	void printSimple(const char str[]);
	std::string getDatetimeStr();
	const char *getRoverDirPath();
	std::string getFileName() { return filename; };

	void setAddRoverLogPath(const char path[]);
};

/*
memo:
writing_file.open(filename, std::ios::app); //std::ios::out=上書き、std::ios::app=追記
*/

#endif
