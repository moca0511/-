#ifndef __MY_TOF_H__
#define __MY_TOF_H__

#include <Wire.h>
#include <VL53L0X.h>

//------------------------------
//変数宣言
//------------------------------

#define SENSOR_NUM 6 // 使用するセンサーの数
#define ADDRESS_DEFALUT 0x29//0b0101001 // 0x29 //デフォルトのtofセンサのアドレス
#define ADDRESS_00 (ADDRESS_DEFALUT + 2)  //設定する一つ目のアドレス
static const int GPIO_MASK_ARRAY[SENSOR_NUM] = {4, 5, 14, 13, 8, 9};  // 各センサのXSHUTへ接続されているGPIOの配列
static VL53L0X gSensor[SENSOR_NUM]; // 使用するセンサークラス配列

bool tofInitErr[SENSOR_NUM];
int tofInitRaw[SENSOR_NUM];
#define TOF_BOUND_BASE 1000
static const int TOF_BOUND_ARRAY[SENSOR_NUM] = {TOF_BOUND_BASE, TOF_BOUND_BASE, TOF_BOUND_BASE, TOF_BOUND_BASE, TOF_BOUND_BASE, TOF_BOUND_BASE};

//マスターと共有する変数
int tofConnectNum = 0;
#define TOF_STATES__NORMAL 0
#define TOF_STATES__ERR_CONNECTION 1
int tofStates = TOF_STATES__NORMAL;

#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3
unsigned char wallFlg[4];

//------------------------------
//関数宣言
//------------------------------
void getTofRawAll() ;
void tofSetup() ;
void getTofRaw(int num);
void checkWall();
bool ifCheckTof(int num);
//------------------------------
//関数
//------------------------------

bool ifCheckTof(int num) {
  if ((tofInitRaw[num] < TOF_BOUND_ARRAY[num]) && (tofInitRaw[num] >= 0))
    return HIGH;
  return LOW;
}

#define WALL_ON 1
#define WALL_OFF 0 


void checkWall() {
  getTofRawAll();

  if (ifCheckTof(0) || ifCheckTof(1)) {
    wallFlg[WALL_FRONT] = WALL_ON;
  } else {
    wallFlg[WALL_FRONT] = WALL_OFF;
  }

  if (ifCheckTof(2) || ifCheckTof(3)) {
    wallFlg[WALL_POST] = WALL_ON;
  } else {
    wallFlg[WALL_POST] = WALL_OFF;
  }

  if ( ifCheckTof(4)) {
    wallFlg[WALL_RIGHT] = WALL_ON;
  } else {
    wallFlg[WALL_RIGHT] = WALL_OFF;
  }

  if ( ifCheckTof(5)) {
    wallFlg[WALL_LEFT] = WALL_ON;
  } else {
    wallFlg[WALL_LEFT] = WALL_OFF;
  }

}

void getTofRawAll() {
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    getTofRaw(i);
    if (tofInitRaw[i] == -2)
      tofStates = TOF_STATES__ERR_CONNECTION;
  }
}

//直接接続
//#define TOF_XSHUT_OFF LOW
//#define TOF_XSHUT_ON HIGH

//notを挟む
#define TOF_XSHUT_OFF HIGH
#define TOF_XSHUT_ON LOW


void tofSetup() {
  // まず全てのGPIOをLOW
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    pinMode(GPIO_MASK_ARRAY[i], OUTPUT);
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_OFF);
  }

  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  delay(500);

  // まず全てのGPIOをLOW
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    pinMode(GPIO_MASK_ARRAY[i], OUTPUT);
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_OFF);
  }

  for (int i = 0; i < SENSOR_NUM; i++) {
    // センサを初期化
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_ON); //picoなど3.3V出力
    //pinMode(GPIO_MASK_ARRAY[i], INPUT);   //arduinoなど5V出力
    delay(1000);
    gSensor[i].setBus(&Wire1);
    if (gSensor[i].init() == true)
    {
      gSensor[i].setTimeout(1000);
      //gSensor[i].startContinuous(50);
      int address = ADDRESS_00 + (i * 1);
      gSensor[i].setAddress(address);
      tofInitErr[i] = LOW;
      tofConnectNum++;
    }
    else
    {
      tofInitErr[i] = HIGH;
    }
    getTofRaw(i);
    Serial.print(i);
    Serial.print(", ");
    Serial.println(tofInitRaw[i]);
    delay(50);
  }
}

/*
  戻り値> 0:センサーの値
  戻り値=-1:接続失敗
  戻り値=-2:タイムアウト
*/
void getTofRaw(int num) {
  int returnData = -1;
  if (num < SENSOR_NUM) {
    if (tofInitErr[num] == LOW) {
      if (tofInitRaw[num] != -2)
      {
        returnData = gSensor[num].readRangeSingleMillimeters();
        if (gSensor[num].timeoutOccurred())
          returnData = -2;
      } else {
        returnData = -2;
      }
    }
  }
  tofInitRaw[num] = returnData;
}

#endif
