#ifndef __MY_TOF_H__
#define __MY_TOF_H__
#include <Wire.h>
#include <VL53L0X.h>

//------------------------------
//センサ管理用変数宣言
//------------------------------
#define SENSOR_NUM 6 // 使用するセンサーの数
#define ADDRESS_DEFALUT 0x40//0b0101001 // 0x29 //デフォルトのtofセンサのアドレス
#define ADDRESS_00 (ADDRESS_DEFALUT + 1)  //設定する一つ目のアドレス
static const int GPIO_MASK_ARRAY[SENSOR_NUM] = {4, 5, 6, 7, 8, 9};  // 各センサのXSHUTへ接続されているGPIOの配列
static VL53L0X gSensor[SENSOR_NUM]; // 使用するセンサークラス配列

bool tofInitErr[SENSOR_NUM];
int tofInitRaw[SENSOR_NUM];

//直接接続
#define TOF_XSHUT_OFF LOW
#define TOF_XSHUT_ON HIGH

/*
  void i2cScan() {
  Serial.println("Scanning...");

  int nDevices = 0;
  byte error, address;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  }
*/

void tofSetup() {
  // まず全てのGPIOをLOW
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    pinMode(GPIO_MASK_ARRAY[i], OUTPUT);
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_OFF);
  }

  //i2c用ピン設定
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();

  delay(100);

  Serial.println("\n-----------\n");

  for (int i = 0; i < SENSOR_NUM; i++) {
    Serial.print(i);
    Serial.print(":");

    // センサを初期化
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_ON); //picoなど3.3V出力
    //pinMode(GPIO_MASK_ARRAY[i], INPUT);   //arduinoなど5V出力
    delay(500);

    if (i > 2)
      gSensor[i].setBus(&Wire1);

    if (gSensor[i].init() == true)
    {
      gSensor[i].setTimeout(1000);
      byte address = ADDRESS_00 + (i * 1);
      gSensor[i].setAddress(address);
      tofInitErr[i] = LOW;
      Serial.println("change_ok");
    }
    else
    {
      Serial.println("change_err");
      tofInitErr[i] = HIGH;
    }
  }
}


/*
  センサーの読み込み(単体)
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


//センサーの読み込み(全て)
void getTofRawAll() {
  for (int i = 0; i < SENSOR_NUM; i++)
    getTofRaw(i);
}


//------------------------------
//wallFlg用
//------------------------------
#define TOF_BOUND_BASE 1000
static const int TOF_BOUND_ARRAY[SENSOR_NUM] = {TOF_BOUND_BASE, TOF_BOUND_BASE / 2, TOF_BOUND_BASE / 2, TOF_BOUND_BASE, TOF_BOUND_BASE / 2, TOF_BOUND_BASE / 2};

/*
  各センサーと要素の向き
  前(0)：0,1
  後(2):2,3
  右(1):4
  左(4):5
*/

#define WALL_FRONT 0
#define WALL_RIGHT 1
#define WALL_POST 2
#define WALL_LEFT 3
#define WALL_FLG_COUNT 4

#define FRONT_SENSER 0
#define LEFT_SENSER1 1
#define LEFT_SENSER2 2
#define POST_SENSER 3
#define RIGHT_SENSER1 4
#define RIGHT_SENSER2 5

unsigned char wallFlg[WALL_FLG_COUNT];


#define WALL_ON 1
#define WALL_OFF 0

//読み込み済みtofデータが基準値より短いか
//戻り値：短ければHIGH
bool ifCheckTof(int num) {
  if ((tofInitRaw[num] < TOF_BOUND_ARRAY[num]) && (tofInitRaw[num] >= 0))
    return HIGH;
  return LOW;
}

//センサーを読み込み、距離が規定値より短いかを判定
//結果をwallFlgに挿入
void checkWall() {
  getTofRawAll();

  if (ifCheckTof(FRONT_SENSER) ) {
    wallFlg[WALL_FRONT] = WALL_ON;
  } else {
    wallFlg[WALL_FRONT] = WALL_OFF;
  }

  if (ifCheckTof(POST_SENSER) ) {
    wallFlg[WALL_POST] = WALL_ON;
  } else {
    wallFlg[WALL_POST] = WALL_OFF;
  }

  if (ifCheckTof(RIGHT_SENSER1) || ifCheckTof(RIGHT_SENSER2)) {
    wallFlg[WALL_RIGHT] = WALL_ON;
  } else {
    wallFlg[WALL_RIGHT] = WALL_OFF;
  }

  if (ifCheckTof(LEFT_SENSER1) || ifCheckTof(LEFT_SENSER2)) {
    wallFlg[WALL_LEFT] = WALL_ON;
  } else {
    wallFlg[WALL_LEFT] = WALL_OFF;
  }

}

#endif
