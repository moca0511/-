//g++ *.cpp -I${PRO4_ROVER_DIR_PATH}/lib -lcurl -Wno-psabi
//g++ *.cpp -I${PRO4_ROVER_DIR_PATH}/lib -lcurl -Wno-psabi && ./a.out
//sudo apt install nlohmann-json-dev
#include "myPostJson/postTcpStruct.h"

#include <iostream>

int main(int argc, char *argv[])
{
	const char url_target[] = "https://httpbin.org/post";
	const char post_data[] = "user=jiro&password=123456";

	//POST_JSONテスト

	MY_POST_JSON B(url_target);

	std::string tag1 = "\"password\": ";

	std::string str_out = B.url_post_proc(url_target, post_data);

	//Json出力
	json j = json::parse(str_out.c_str());
	std::string s = j.dump(4);
	std::cout << s << "\n";

	//キーデータ出力
	std::string keyStr = "/form/user";
	std::string CCC = j[json::json_pointer(keyStr.c_str())];
	std::cout << CCC << std::endl;

	std::cout << "\npoint start\n\n";

	LON_LAT send_LanLat;
	send_LanLat.now.point.lon = 135.5;
	send_LanLat.now.point.lat = 35.5;
	send_LanLat.rtkResult = 1;

	double sendDir = 3.0, sendSpeed = 10.0, sendErrmsg = 1;
	tcpStruct readData;
	MY_POST_JSON execution(SERVER_URL);
	makeSendData_execution(&execution, send_LanLat, sendDir, sendSpeed, sendErrmsg);
	std::cout << execution.getSendData() << "\n";
	execution.requestPost();
	execution.printDump();

	std::cout << "\n execution start\n";
	postExecution(send_LanLat, sendDir, sendSpeed, sendErrmsg, &readData);

	return 0;
}
