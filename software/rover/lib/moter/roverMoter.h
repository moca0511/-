#ifndef _ROVER_MOTER_H_
#define _ROVER_MOTER_H_
#include "moter/pwmLAP.h"
#include "tcpServerClient/tcpStruct.h"

#define MOTER_STRUCT msg3

#define DIR_PIN_1 12
#define PWM_PIN_1 26
#define DIR_PIN_2 13
#define PWM_PIN_2 6

#define DIR_PIN_R DIR_PIN_2
#define PWM_PIN_R PWM_PIN_2
#define DIR_PIN_L DIR_PIN_1
#define PWM_PIN_L PWM_PIN_1

#define M_OPERATION_S 0	 // スリープ
#define M_OPERATION_B 1	 // ブレーキ
#define M_OPERATION_MP 2 // 前進	//move progress
#define M_OPERATION_MB 3 // 後進	//move back
#define M_OPERATION_TR 4 // 右回転	//turn Right
#define M_OPERATION_TL 5 // 左回転	//turn left

#define ROTATION_REGULAR_MAX 255
#define ROTATION_INVERSE_MAX 0

#define ROTATION_REGULAR_HALF 205
#define ROTATION_INVERSE_HALF 50

#define ROTATION_REGULAR_LOW 155
#define ROTATION_INVERSE_LOW 100

#define POWER_MAX 0
#define POWER_HALF 0
#define POWER_LOW 0

class roverMoter
{
private:
public:
	MoterDriver10A L;
	MoterDriver10A R;
	roverMoter();
	~roverMoter();
	void moterSetup();
};
int moter2operation(int mL, int mR);
void operation2moter(int mOp, MOTER_STRUCT *moter, int time_ms, const int powerMode = POWER_MAX);

#endif
