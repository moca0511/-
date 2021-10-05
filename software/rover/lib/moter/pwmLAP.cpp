#include "pwmLAP.h"
char wiringSetupFlg=false;

MoterDriver10A::MoterDriver10A(int pwmPin, int dirPin)
{
	if(wiringSetupFlg==false)
	{
		wiringPiSetupGpio();
		wiringSetupFlg = true;
	}
	pwm_Pin = pwmPin;
	dir_Pin = dirPin;
	PwmRange = DEF_PWM_RANGE;
	PwmClock = DEF_PWM_CLOCK;
	setup();
}

MoterDriver10A::~MoterDriver10A()
{
#ifndef PROGRAM_END_AFTER_PWM_NOT_STOP
	rotation(-1);
#endif
}

//モータドライバ用の各種設定
void MoterDriver10A::setup()
{
	//Dirピン
	pinMode(dir_Pin, PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(PwmClock);
	pwmSetRange(PwmRange);
	//PWMピン
	pinMode(pwm_Pin, OUTPUT);
	//空回転の設定
	digitalWrite(pwm_Pin, 0);
}

//モータを回転させる(outRegularによって正回転が変化)
//引数：0~PwmRange/2の範囲は正回転。(PwmRange/2)=ブレーキ。PwmRange/2~PwmRangeの範囲は逆回転、0未満は空回転
//戻り値：動作成功=0,動作失敗(空回転)=1
int MoterDriver10A::rotation(int data)
{
	if ((data < PwmRange) && (0 <= data))
	{

		if (outRegular == 1)
			data = PwmRange - data;
		pwmWrite(dir_Pin, data);
		digitalWrite(pwm_Pin, 1);
		return 0;
	}
	digitalWrite(pwm_Pin, 0);
	return 1;
}

//基準回転方向の設定
//引数：0,1=2種類の回転方向から設定
//戻り値：設定成功=0,設定失敗=1
int MoterDriver10A::setRotationDirection(int num)
{
	switch (num)
	{
	case 0:
		outRegular = 1;
		break;
	case 1:
		outRegular = 0;
		break;
	default:
		return 1;
	}
	return 0;
}
