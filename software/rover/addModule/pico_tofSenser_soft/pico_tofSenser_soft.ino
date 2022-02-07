#include <SoftWire.h>
#include <VL53L0X_Soft.h>

//SoftWire i2c_soft = SoftWire(0, 1); //sda,scl
//SoftWire i2c_soft1 = SoftWire(2, 3); //sda,scl
SoftWire i2c_soft(0, 1); //sda,scl
SoftWire i2c_soft1(2, 3); //sda,scl


//------------------------------
//変数宣言
//------------------------------

#define SENSOR_NUM 6 // 使用するセンサーの数
#define ADDRESS_DEFALUT 0x40//0b0101001 // 0x29 //デフォルトのtofセンサのアドレス
#define ADDRESS_00 (ADDRESS_DEFALUT + 1)  //設定する一つ目のアドレス
static const int GPIO_MASK_ARRAY[SENSOR_NUM] = {4, 5, 6, 7, 8, 9};  // 各センサのXSHUTへ接続されているGPIOの配列
static VL53L0X_SOFT gSensor[SENSOR_NUM];; // 使用するセンサークラス配列

bool tofInitErr[SENSOR_NUM];
int tofInitRaw[SENSOR_NUM];


//直接接続
//#define TOF_XSHUT_OFF LOW
//#define TOF_XSHUT_ON HIGH

//notを挟む
#define TOF_XSHUT_OFF HIGH
#define TOF_XSHUT_ON LOW

void i2cScan(SoftWire *bus) {
  Serial.println("Scanning...");

  int nDevices = 0;
  byte error, address;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    bus->beginTransmission(address);
    error = bus->endTransmission();

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

void setup()
{
  delay(100);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  Serial.begin(9600);
  delay(100);

  // まず全てのGPIOをLOW
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    pinMode(GPIO_MASK_ARRAY[i], OUTPUT);
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_OFF);
  }
  delay(100);


  i2c_soft.begin();
  i2c_soft1.begin();

  i2cScan(&i2c_soft);
  Serial.println("\n");
  i2cScan(&i2c_soft1);

  delay(100);

  Serial.println("\n-----------\n");

  for (int i = 0; i < SENSOR_NUM; i++) {
    Serial.println(i);

    // センサを初期化
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_ON); //picoなど3.3V出力
    //pinMode(GPIO_MASK_ARRAY[i], INPUT);   //arduinoなど5V出力
    delay(500);
    //i2cScan();
    Serial.println("scan_e");

    if (i > 2)
      gSensor[i].setBus(&i2c_soft);
    else
      gSensor[i].setBus(&i2c_soft1);

    if (gSensor[i].init() == true)
    {
      Serial.println("change_1");
      gSensor[i].setTimeout(1000);
      //gSensor[i].startContinuous(10);
      byte address = ADDRESS_00 + (i * 1);
      Serial.println("change_2");
      gSensor[i].setAddress(address);
      tofInitErr[i] = LOW;
    }
    else
    {
      Serial.println("change_err");
      tofInitErr[i] = HIGH;
    }


    //i2cScan(&i2c_soft);
    Serial.println("set end\n");
    delay(100);
  }

  for (int i = 0; i < SENSOR_NUM; i++)
    digitalWrite(GPIO_MASK_ARRAY[i], TOF_XSHUT_ON); //picoなど3.3V出力
  //pinMode(GPIO_MASK_ARRAY[i], INPUT);   //arduinoなど5V出力


  delay(100);

  //i2cScan();
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

void loop()
{
  /*
    while (1) {
    i2cScan();
    delay(100);
    }
  */
  for (int i = 0; i < SENSOR_NUM; i++)
    getTofRaw(i);
  for (int i = 0; i < SENSOR_NUM; i++) {
    Serial.print(tofInitRaw[i]);
    Serial.print(", ");
  }
  Serial.println("  ");
  delay(100);
}
