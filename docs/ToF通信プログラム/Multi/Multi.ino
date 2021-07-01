#include <Wire.h>
#include <VL53L0X.h>

#define SENSOR_NUM 2 // 使用するセンサーの数
#define ADDRESS_DEFALUT 0b0101001 // 0x29
#define ADDRESS_00 (ADDRESS_DEFALUT + 2)
/**
 * 各センサのXSHUTへ接続されているGPIOの配列
 */
const int GPIO_MASK_ARRAY[SENSOR_NUM] = {2, 3}; /*, 18, 17};*/
VL53L0X gSensor[SENSOR_NUM]; // 使用するセンサークラス配列

void setup() {
  Serial.begin(9600);
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  
  // まず全てのGPIOをLOW
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    pinMode(GPIO_MASK_ARRAY[i], OUTPUT);
    digitalWrite(GPIO_MASK_ARRAY[i], LOW);
  }

  for (int i = 0; i < SENSOR_NUM; i++) {
    // センサを初期化
    pinMode(GPIO_MASK_ARRAY[i], INPUT);
    if (gSensor[i].init() == true)
    {
      gSensor[i].setTimeout(1000);
      gSensor[i].startContinuous(10);
      int address = ADDRESS_00 + (i * 2);
      gSensor[i].setAddress(address);
    }
    else
    {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.println(" error");
    }
  }
}

void loop() {
  for (int i = 0; i < SENSOR_NUM; i++) {
    Serial.print(gSensor[i].readRangeSingleMillimeters());
    if (gSensor[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Serial.println();
    delay(500);
  }
}
