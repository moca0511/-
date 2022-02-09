#ifndef PWM_LAP_H
#define PWM_LAP_H

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include <unistd.h>

/*
SMBではなくLAP方式のPWM制御を行っている。
よって、ブレーキが実行可能。
DIR_PINからpwm出力が出ている。
PWM_PINはHIGHかLOWを出力
*/

#define DEF_PWM_RANGE 256
#define DEF_PWM_CLOCK 100

//#define PROGRAM_END_AFTER_PWM_NOT_STOP	//プログラム終了後にモータを停止したいときはコメントアウト

//クラスを作成する前にwiringPiSetupGpio()が必須。
class MoterDriver10A
{
private:
	int pwm_Pin;  //PWMピン
	int dir_Pin;  //dirピン(回転方向の決定)
	int PwmRange; //pwm出力の段階値
	int PwmClock; //pwmの周波数(HZ？)
	int outRegular = 1;
	void setup();

public:
	MoterDriver10A(int pwmPin, int dirPin);
	~MoterDriver10A();
	int rotation(int data);
	int setRotationDirection(int num);
	void sleep() { rotation(-1); };
	void stop() { rotation(PwmRange / 2); };
};
#endif
