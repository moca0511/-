#include "logFile/myLog.h"

//#define LOG_FILE_PATH "./log/log.log"
myLogFile B;
int main(int argc, char *argv[])
{
	std::string A = "AAA\n";

	//myLogFile B(LOG_FILE_PATH);

	B.print(A);
	A = "BBB\n";
	B.print(A.c_str());

	//B.fileInit();

	A = "CCCCC";
	B.print(A.c_str());

	//printf("%s\n", B.getRoverDirPath());
	//B.setRoverLogPath();
	printf("%s\n", B.getDatetimeStr().c_str());
	printf("%s\n", B.filename.c_str());

	return 0;
}
