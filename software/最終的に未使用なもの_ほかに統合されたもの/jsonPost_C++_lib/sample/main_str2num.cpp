//sudo apt install nlohmann-json-dev

#include "jsonPost/myJsonRW.h"
#include <iostream>

const json sampleData = {
	{"args", {}},
	{"data", ""},
	{"files", {}},
	{"form", {
				 {"password", "123456"},
				 {"user", "jiro"},
				 {"double", "11.1"},
			 }},
	{"headers", {
					{"Accept", "*/*"},
					{"Content-Length", "25"},
					{"Content-Type", "application/x-www-form-urlencoded"},
					{"Host", "httpbin.org"},
					{"X-Amzn-Trace-Id", "Root=1-61824db1-46bc39706f01f89f0cc33475"},
				}},
	{"json", nullptr},
	{"origin", "14.13.199.129"},
	{"url", "https://httpbin.org/post"}};

#define JOSN_READ_TAG__PARENT_TAG "/form/"

int main(int argc, char *argv[])
{
	//POST_JSONテスト
	MY_READ_JSON a;

	//std::cout << sampleData << "\n";

	a.setReadData(sampleData);
	int num1 = a.getTagData(JOSN_READ_TAG__PARENT_TAG "password");
	int num2 = a.getTagData(JOSN_READ_TAG__PARENT_TAG "password", MY_INT);
	double num3 = a.getTagData(JOSN_READ_TAG__PARENT_TAG "double", MY_DOUBLE);
	std::string num4 = "a";
	a.getTagData(JOSN_READ_TAG__PARENT_TAG "user", &num4);

	std::cout << a.getReadData().dump(4) << "\n\n";

	std::cout
		<< num1 << ", "
		<< num2 << ", "
		<< num3 << ", "
		<< num4 << ", "
		<< "\n";
}
