//sudo apt install nlohmann-json-dev

#include "jsonPost/postJson.h"
#include <iostream>

const char url_target[] = "https://httpbin.org/post";
const char post_data[] = "user=jiro&password=123456&double=11.1";

int main(int argc, char *argv[])
{
	//関数宣言
	MY_POST_JSON_SIMPLE B;

	B.url_post_proc(url_target, post_data);

	std::cout << post_data << "\n";

	std::cout
		<< "raw \n"
		<< B.getPostDataString() << "\n\n"
		<< "json\n"
		<< B.getPostDataJson() << "\n\n";
}
