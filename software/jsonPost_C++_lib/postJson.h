#ifndef _POST_JSON_H_
#define _POST_JSON_H_
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class MY_POST_JSON_SIMPLE
{
private:
	std::string postData;

public:
	std::string url_post_proc(const char url[], const char post_data[]);
	json getPostDataJson() { return json::parse(postData.c_str()); };
	std::string getPostDataString() { return postData; };
};

#endif
