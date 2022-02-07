/*
正常終了でしか、書き込まれないので注意
*/

#ifndef _INA_260_H_
#define _INA_260_H_

#include <sstream>
#include <fstream>
#include <wiringPiI2C.h>
#define INA_PIN_GNA 0
#define INA_PIN_VS 1
#define INA_PIN_SDA 2
#define INA_PIN_SCL 3

/*
GND GND 1000000
GND VS 1000001
GND SDA 1000010
GND SCL 1000011
VS GND 1000 100
VS VS 1000101
VS SDA 1000110
VS SCL 1000111
SDA GND 1001000
SDA VS 1001001
SDA SDA 1001010
SDA SCL 1001011
SCL GND 1001100
SCL VS 1001101
SCL SDA 1001110
SCL SCL 1001111

GND GND 0000
GND VS 0001
GND SDA 0010
GND SCL 0011
VS GND 0100
VS VS 0101
VS SDA 0110
VS SCL 0111
SDA GND 1000
SDA VS 1001
SDA SDA 1010
SDA SCL 1011
SCL GND 1100
SCL VS 1101
SCL SDA 1110
SCL SCL 1111
*/

class INA
{
private:
	int ina_address;
	int errFlg = 0;
	int fdINA;
	int init(const int add);

public:
	INA(const int A0, const int A1, const bool flgPrint = true);
	INA(const int add, const bool flgPrint = true);
	int getErrFlg() { return errFlg; };
	double get_Data(const int reg);
	int getAdress() { return ina_address; };
	double get_mA();
	double get_V();
	double get_P();
};

#endif
