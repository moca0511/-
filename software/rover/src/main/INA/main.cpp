#include <unistd.h>
#include "logFile/myLog.h"

//------------------------------
// main関数
//------------------------------
void main_rover();
std::string argv_string;
#include "INA260/INA260.h"
#include "logFile/myLog.h"

#include <unistd.h>
void main_inaLog(const bool flgPrint);

int main(int argc, char *argv[])
{

	printf("start InaLog\n");
	main_inaLog(true);
	printf("end InaLog\n");

	return 0;
}

void main_inaLog(const bool flgPrint)
{
	char temp[60];
	INA INA_1(0x40, false);
	//INA INA_2(0x44, false);
	if (INA_1.getErrFlg() == -1)
	{
		printf("INA_1:connect err\n");
		return;
	}
	/*if (INA_2.getErrFlg() == -1)
	{
		printf("INA_2:connect err\n");
		return;
	}
*/
	myLogFile LOG_INA("./log/INA_1.log", false);
	LOG_INA.printSimple("\n");
	while (1)
	{
		//sprintf(temp, "0x%x, %4.2fv, %4.2fv,\t0x%x, %4.2fv, %4.2fv\n", INA_1.getAdress(), INA_1.get_V(), INA_1.get_mA(), INA_2.getAdress(), INA_2.get_V(), INA_2.get_mA());
		sprintf(temp, "0x%x, %4.2fv, %4.2fmA\n", INA_1.getAdress(), INA_1.get_V(), INA_1.get_mA());

		if (flgPrint)
			printf("%s", temp);
		LOG_INA.print(temp);
		sleep(1);
	}
}
