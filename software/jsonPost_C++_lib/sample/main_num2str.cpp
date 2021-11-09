#include "jsonPost/myJsonRW.h"
#include <iostream>

const double sendDir = 3.2;
const char str[] = "ABCD";
const int num = 234;
const bool on = true;
const bool off = false;

int main(int argc, char *argv[])
{

	MY_WRITE_JSON AAA;

	AAA.addTagData("int", num);
	AAA.addTagData("double", sendDir);
	AAA.addTagData("char", str);
	AAA.addTagData("on", on);
	AAA.addTagData("off", off);
	std::cout << AAA.getWriteData() << "\n";


	AAA.clear();
	std::cout << AAA.getWriteData() << "\n";

	return 0;
}
