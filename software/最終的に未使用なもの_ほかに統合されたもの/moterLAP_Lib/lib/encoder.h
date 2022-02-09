
////g++ encoder.cpp -lwiringPi
//https://www.raspberrypi.org/forums/viewtopic.php?t=184388

#ifndef ENCODER_H
#define ENOCDER_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define ENCODER_COUNT 2 //接続するエンコーダの数
#define ENCODER_NUM0 0
#define ENCODER_NUM1 1

class Encoder
{
private:
	int PIN_NUM_A;
	int PIN_NUM_B;
	char oldPinA = 0;
	char oldPinB = 0;
	signed long int encoder_raw_count = 0;
	int rotationDirection = 1;

public:
	Encoder(int pinA, int pinB, int num);
	~Encoder();
	void rotary_changedPin(void);
	int setRotationDirection(int num);
	void init();
	void print();
	signed long int readRaw();
};

#endif
