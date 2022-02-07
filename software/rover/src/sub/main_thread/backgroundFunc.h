#ifndef _BACKGROUND_FUNC_H_
#define _BACKGROUND_FUNC_H_
//#include "myFork/myFork.h"

#define FUNC_NUM 5

#define SERVER_LODING_TIME 2 //初期に動作させるものを待つ時間(s)
#define SERVER_LODING_TIME_MAIN 10 //forkで初期に動作させるものを待つ時間(s)

#define FUNC__ROVER FUNC_NUM
//強制終了を行うfuncの番号
#define FUNC__INA_LOG 3
#define FUNC__TEMP_LOG 4
#define FUNC__SERVER 0

#include<thread>
#include <cstdio>
#include <cstdint>

extern void (*const backgroundFunc[FUNC_NUM])(void);

//ina260のログを記録用
void main_inaLog(const bool flgPrint = true);
//consoleM5stickCの動作プログラム
int consoleM5stickC(int n_time);

void main_tempLog();

//バックグラウンドで動作する関数
void backgroundServer();
void backgroundF9p();
void backGraundM5();
void backGraundInaLog();
void backGraundTemp();

extern uint32_t end_flag_;

#endif
