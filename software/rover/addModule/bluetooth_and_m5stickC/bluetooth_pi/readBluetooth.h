#ifndef _READ_BLUETOOTH_H_
#define _READ_BLUETOOTH_H_

#define BLUETOOTH_ADDR "50:02:91:A1:58:86"

class myBluetooth
{
private:
	int fd;
	int get_char = 0;	// 取得した文字列
	int sendFlg = 0;	// データ受信したときを0としたときの、送信回数。
						//MAX=SEND_COUNT_MAXであり、SEND_COUNT_MAXになると、1に戻る
	int checkFlg = 0;	//データを送信したとき0になる。CHECHK_COUNT_MAXでcheckデータ送信。
						//mySend_check()の度に増加。

	bool checkFileExistence(const char str[]);

public:
	myBluetooth();
	int myRead();
	void mySend(const int);
	int mySend_check();
	int getSendFlg() { return sendFlg; };
	void mySend_check_force();
};
#endif
