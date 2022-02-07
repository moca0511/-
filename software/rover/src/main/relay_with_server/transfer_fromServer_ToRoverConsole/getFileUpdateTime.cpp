#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <cstring>
#include <string.h>
#include <iostream>

double getFileUpdateTime(std::string fileName);

int main(int argc, char *argv[])
{

	/* argc check */
	if (argc != 2)
	{
		printf("USAGE:%s <FILE PATH>\n", argv[0]);
		return 1;
	}
	double last = 0, now;
	while (1)
	{
		now = getFileUpdateTime(argv[1]);
		if (last != now)
		{
			printf("%lf\n", now);
			last = now;
		}
	}
	return 0;
}

double getFileUpdateTime(std::string fileName)
{
	int i;
	struct stat st;
	char date_str[256];
	char result_str[100];
	/* get stat */
	if (stat(fileName.c_str(), &st) != 0)
	{
		printf("%s\n", strerror(errno));
		return 1;
	}

	/* show */
	strftime(date_str, 255, "%Y%m%d%H%I%M%S", localtime(&st.st_mtime));
	sprintf(result_str, "%s.%09ld\n", date_str, st.st_mtim.tv_nsec);
	return std::stod(result_str);
}
