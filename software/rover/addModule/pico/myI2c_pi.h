#ifndef __MY_I2C_PI_H__
#define __MY_I2C_PI_H__

#include <Wire.h>

#define I2C_DATA_SIZE 0x06
#define I2C_ADDRESS 0x08

unsigned char i2cData[I2C_DATA_SIZE];
unsigned char i2cNoElement = 0;

//------------------------------
// 関数宣言
//------------------------------
void requestEvent();
void receiveEvent(int a) ;
void myI2C_pi_setup();

//------------------------------
// 関数
//------------------------------

void requestEvent() {
  Wire1.write(i2cData[i2cNoElement]);
}

void receiveEvent(int a) {
  int sub;
  i2cNoElement = Wire1.read();
  int i2cDataSize = Wire1.available();
  for (int i = 0; i < i2cDataSize; i++) {
    sub = Wire1.read();
  }
}

void myI2C_pi_setup(){
  Wire1.setSDA(2);
  Wire1.setSCL(3);

  Wire1.begin(I2C_ADDRESS);

  Wire1.onRequest(requestEvent);
  Wire1.onReceive(receiveEvent);
}


#endif
