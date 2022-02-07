#ifndef _MY_JSON_H_
#define _MY_JSON_H_
#include "jsonPost/myJsonRW.h"
#include "jsonPost/postJson.h"
#include <stdio.h>
#include <sys/time.h>

class MY_JSON : public MY_READ_JSON, public MY_WRITE_JSON, private MY_POST_JSON_SIMPLE
{
private:
	std::string url;

	//経過時間測定用変数
	unsigned int sec;
	int nsec;
	double d_sec = 0;
	struct timespec start_time, end_time;
	void timeCalculation()
	{
		sec = end_time.tv_sec - start_time.tv_sec;
		nsec = end_time.tv_nsec - start_time.tv_nsec;
		d_sec = (double)sec + (double)nsec / (1000 * 1000 * 1000);
	};

public:
	MY_JSON(const char _url[]) { url = _url; };
	void send()
	{
		clock_gettime(CLOCK_REALTIME, &start_time);
		url_post_proc(url.c_str(), getWriteData().c_str());
		setReadData(getPostDataJson());
		setClearFlg_writeData();
		clock_gettime(CLOCK_REALTIME, &end_time);
		timeCalculation();
	};
	double getPastTime() { return d_sec; };
};

#endif
