#ifndef _PICO_QUICK_H_
#define _PICO_QUICK_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

#include <iostream>
#include <algorithm>
#include <iterator>

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/time.h>
#include <sys/resource.h>

//------------------------------
//gpio 接続
//------------------------------

// set_FILENAME()において設定可能なファイルパスのモード
#define FILE_CONTROL_EXPORT 0
#define FILE_CONTROL_DIRECTION 1
#define FILE_CONTROL_EDGE 2
#define FILE_CONTROL_VALUE 3
#define FILE_CONTROL_UNEXPORT 4
#define FILE_CONTROL_DIR 5

// set()において設定可能な読み書きモード
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1

class myGpio
{
private:
	int fd;
	int _mode;
	int gpioPin; // クラスによって処理を行うgpioPin
	bool setFlg; // gpioの初期が行われていればtrue
	std::string set_FILENAME(int type, int num=0);
	void setModeIN();
	void writeGpioNum();

public:
	myGpio() { setFlg = false; };
	myGpio(int num, int mode = GPIO_MODE_IN) { set(num, mode); }; //gpioの初期化, setで代用化
	~myGpio();
	void set(int, int = GPIO_MODE_IN); 
	bool myRead();
	void myWrite(bool);	//gpioの書き込み(未作成,setにおいてgpioの設定を行う必要がある)
};

//------------------------------
//picoｎpin 接続
//------------------------------

#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3

#define PIN_WALL_FRONT 10
#define PIN_WALL_RIGHT 9
#define PIN_WALL_POST 11
#define PIN_WALL_LEFT 5

class pico_quick
{
private:
	int wallFlg[4];
	myGpio readGpio[4];

public:
	pico_quick();
	void getPico(); 
	int getWallFlg(int num) { return wallFlg[num]; };
};

#endif
