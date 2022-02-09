#include "jsonPost/myJson.h"

#define JOSN_READ_TAG__PARENT_TAG "/form/"

const double sendDir = 3.2;
const char str[] = "ABCD";
const int num = 234;
const bool on = true;
const bool off = false;

int main(int argc, char *argv[])
{
	MY_JSON FFF("https://httpbin.org/post");

	FFF.addTagData("int", num);
	FFF.addTagData("double", sendDir);
	FFF.addTagData("char", str);
	FFF.addTagData("on", on);
	FFF.addTagData("off", off);

	FFF.addTagData("password", "122");
	FFF.addTagData("user", "naoa");
	std::cout << FFF.getWriteData() << "\n";

	//送信
	FFF.send();
	std::cout << FFF.getReadData().dump(4);
	std::cout << "\n\n";

	//読み込み
	int num1 = FFF.getTagData(JOSN_READ_TAG__PARENT_TAG "password");
	int num2 = FFF.getTagData(JOSN_READ_TAG__PARENT_TAG "password", MY_INT);
	double num3 = FFF.getTagData(JOSN_READ_TAG__PARENT_TAG "double", MY_DOUBLE);
	std::string num4 = "a";
	FFF.getTagData(JOSN_READ_TAG__PARENT_TAG "user", &num4);

	std::cout
		<< num1 << ", "
		<< num2 << ", "
		<< num3 << ", "
		<< num4 << ", "
		<< "\n";

	return 0;
}
