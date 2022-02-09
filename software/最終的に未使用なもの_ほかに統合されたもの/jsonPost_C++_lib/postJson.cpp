#include "jsonPost/postJson.h"
#include <iostream>
#include <cstring>

#include <curl/curl.h>

using namespace std;

//関数宣言
size_t callBackFunk(char *ptr, size_t size, size_t nmemb, std::string *stream);

//関数部
size_t callBackFunk(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
	int realsize = size * nmemb;
	stream->append(ptr, realsize);
	return realsize;
}

string MY_POST_JSON_SIMPLE::url_post_proc(const char url[], const char post_data[])
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	string chunk;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(post_data));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string *)&chunk);
		curl_easy_setopt(curl, CURLOPT_PROXY, "");
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	if (res != CURLE_OK)
	{
		cout << "curl error" << endl;
		exit(1);
	}
	postData = chunk;
	return chunk;
}
