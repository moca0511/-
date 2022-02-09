#include "encoder.h"
//------------------------------
//エンコーダを増やした場合、下記の範囲に修正が必要
//------------------------------

static Encoder *savedInterruptPointer[ENCODER_COUNT];

void helpInterrupt0()
{
	savedInterruptPointer[0]->rotary_changedPin();
}

void helpInterrupt1()
{
	savedInterruptPointer[1]->rotary_changedPin();
}

void (*helpInterruptFunction[ENCODER_COUNT])(void) = {helpInterrupt0, helpInterrupt1};

//------------------------------
Encoder::Encoder(int pinA, int pinB, int num)
{
	PIN_NUM_A = pinA;
	PIN_NUM_B = pinB;
	pinMode(PIN_NUM_A, INPUT);
	pinMode(PIN_NUM_B, INPUT);
	savedInterruptPointer[num] = this;
	wiringPiISR(PIN_NUM_A, INT_EDGE_BOTH, helpInterruptFunction[num]);
	wiringPiISR(PIN_NUM_B, INT_EDGE_BOTH, helpInterruptFunction[num]);
}

Encoder::~Encoder()
{
}

/*
エンコーダ用のクラスに保存されたデータを初期化。
*/
void Encoder::init()
{
	oldPinA = 0;
	oldPinB = 0;
	encoder_raw_count = 0;
}

/*
割り込みで受診したデータを表示
*/
void Encoder::print()
{
	printf("%d\n", readRaw());
}

/*
割り込みでカウントしたパルスを引数で返す。
*/
signed long int Encoder::readRaw()
{
	return encoder_raw_count;
}

/*
割り込み時に実行する関数。パルスをカウントする。
*/
void Encoder::rotary_changedPin(void)
{
	char sigPinA = digitalRead(PIN_NUM_A);
	char sigPinB = digitalRead(PIN_NUM_B);
	int add;
	add = 0;
	if (!sigPinA && !sigPinB)
	{
		if (!oldPinA && oldPinB)
		{
			add = 1;
		}
		else if (oldPinA && !oldPinB)
		{
			add = -1;
		}
		encoder_raw_count = encoder_raw_count + (signed long)(add * rotationDirection);
	}
	oldPinA = sigPinA;
	oldPinB = sigPinB;
}

//基準回転方向の設定
//引数：0,1=2種類の回転方向から設定
//戻り値：設定成功=0,設定失敗=1
int Encoder::setRotationDirection(int num)
{
	switch (num)
	{
	case 0:
		rotationDirection = 1;
		break;
	case 1:
		rotationDirection = -1;
		break;
	default:
		return 1;
	}
	return 0;
}
