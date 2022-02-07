

#include "myTof.h"
#include "myI2C_pi.h"

//表示モード設定
#define SERIAL_PRINT_PC_ALL //PCモード時に全データ表示
#define SERIAL_PRINT_PI		//PIモード時にシリアルプリントも実行

//システムモード設定
//sendMODEの強制的な設定 //優先度PI>PC
//#define FORCE_SEND_MODE_PI
//#define FORCE_SEND_MODE_PC

// 送信先モード
#define SEND_MODE_PIN 18
#define SEND_MODE_PI LOW
#define SEND_MODE_PC HIGH
bool sendMode = SEND_MODE_PC;

const char wallDirection[4][20] = {
  "front=",
  "right=",
  "post =",
  "left ="
};

int ledCount = 0;
bool nowLed = LOW;
int serialCount;
char buff[50];
int ledDelayTime;

void setup()
{
  pinMode(25, OUTPUT);
  pinMode(15, OUTPUT);
    digitalWrite(25, HIGH);
    digitalWrite(15, HIGH);
    Serial.begin(9600);

  pinMode(SEND_MODE_PIN, INPUT_PULLUP);

#ifdef FORCE_SEND_MODE_PI
  sendMode = SEND_MODE_PI;
#elif FORCE_SEND_MODE_PC
  sendMode = SEND_MODE_PC;
#else
  if (digitalRead(SEND_MODE_PIN) != SEND_MODE_PC)
  {
    sendMode = SEND_MODE_PI;
  }
#endif

  tofSetup();

  if (sendMode == SEND_MODE_PI)
    myI2C_pi_setup();

  //　i2cデータの確認用初期化
  for (int i = 0; i < I2C_DATA_SIZE; i++)
    i2cData[i] = i;

  // 内蔵LED設定
  pinMode(25, OUTPUT);

  if (sendMode == SEND_MODE_PC)
  {
    ledDelayTime = 100;
  }
  else
  {
    ledDelayTime = 20;
  }
}

void loop()
{
  if (ledCount >= ledDelayTime)
  {
    ledCount = 0;
    nowLed = !nowLed;
    digitalWrite(25, nowLed); // 内蔵LED点滅
  }

  if (ledCount % 10 == 0)
  {
    checkWall();
    if (sendMode == SEND_MODE_PC)
    {
      sendModePC_Print();
    }
    else
    {
      i2cData[0] = tofConnectNum;
      i2cData[1] = tofStates;
      for (int i = 0; i < 4; i++)
      {
        i2cData[i + 2] = wallFlg[i];
      }

#ifdef SERIAL_PRINT_PI
      sendModePI_Print();
#endif
    }
    serialCount++;
  }
  delay(10);
  ledCount++;
}

void sendModePI_Print()
{
  sprintf(buff, "sendPC %4d: ", serialCount);
  Serial.print(buff);
  for (int i = 0; i < I2C_DATA_SIZE; i++)
  {
    Serial.print(i2cData[i]);
    Serial.print(", ");
  }
  Serial.println();
}

//sendModePCでのシリアルプリントに対する出力
void sendModePC_Print()
{
  sprintf(buff, "sendPC %4d, ", serialCount);
  Serial.print(buff);

#ifdef SERIAL_PRINT_PC_ALL
/*
  for (int i = 0; i < I2C_DATA_SIZE; i++)
  {
    Serial.print("0x");
    Serial.print(i2cData[i], HEX);
    Serial.print(", ");
  }
*/
  Serial.print("add: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sprintf(buff, "0x%2x ", gSensor[i].getAddress());
    Serial.print(buff);
  }
  /*
  Serial.print("e: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sprintf(buff, "%1d, ", tofInitErr[i]);
    Serial.print(buff);
  }
  */
  Serial.print("tofRaw: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sprintf(buff, "%4d, ", tofInitRaw[i]);
    Serial.print(buff);
  }

/*
  Serial.print("tofCheck: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    Serial.print(ifCheckTof(i));
    Serial.print(", ");
  }
*/
#endif

  Serial.print("tofConnectNum ");
  Serial.print(tofConnectNum);
  Serial.print(", ");
  Serial.print("tofStates ");
  Serial.print(tofStates);
  Serial.print(", ");

/*
  Serial.print("wall: ");

  for (int i = 0; i < 4; i++)
  {
    Serial.print(wallDirection[i]);
    if (wallFlg[i])
      Serial.print(" on");
    else
      Serial.print("off");
    Serial.print(", ");
  }
  */

  Serial.println();

  
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    if(tofInitRaw[i]==-2){
      
    }
  }

    delay(10);
}
