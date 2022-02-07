#include <stdio.h>
#define RECV_JSON "/tmp/send.json"
void writeSendJson(int, double, double);

int main()
{

	writeSendJson(1, 2.3, 4.5);
	return 0;
}

void writeSendJson(int a, double b, double c)
{

	FILE *fp;

	//「fprintfExample.txt」のファイルを書き込みモード（"w"）で開く
	fp = fopen(RECV_JSON, "w");

	//「fprintfExample.txt」に「aは100です」と書き込む

	char tmp[256];
	sprintf(tmp, "{\
        \"mode\": %d,\
        \"lat\": %f,\
        \"lng\": %f\
		}",
			a, b, c);
	fprintf(fp, tmp);

	//「fprintfExample.txt」のファイルを閉じる
	fclose(fp);
}
