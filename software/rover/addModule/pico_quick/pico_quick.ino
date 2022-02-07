#include "myTof.h"
#include "myI2C_pi.h"



// 送信先モ選択
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
#define  ledDelayTime 100

//------------------------------
//main
//------------------------------
void setup()
{
  // 内蔵LED設定
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  Serial.begin(9600);

  //スイッチ読み込み
  pinMode(SEND_MODE_PIN, INPUT_PULLUP);


  if (digitalRead(SEND_MODE_PIN) != SEND_MODE_PC)
    sendMode = SEND_MODE_PI;
  else
    sendMode = SEND_MODE_PC;


  //センサ初期設定
  setup_Pi_Out();
  tofSetup();

}

void loop()
{ if (ledCount >= ledDelayTime)
  {
    ledCount = 0;
    nowLed = !nowLed;
    digitalWrite(25, nowLed); // 内蔵LED点滅
  }
  if (sendMode == SEND_MODE_PI)
    mainLoop();
  else
    testLoop2();


}


//------------------------------
//loop
//------------------------------
void mainLoop() {
  if (ledCount % 10 == 0)
  {
    checkWall();
    outpudFromArray(wallFlg);
    sendModePC_Print();
    serialCount++;
  }
  delay(10);
  ledCount++;
}


void testLoop2() {
  for (int i = 0; i < PI_OUT_NUM; i++)
    digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_OFF);
  delay(1000);

  for (int i = 0; i < PI_OUT_NUM; i++)
  {
    digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_ON);
    delay(1000);

    //初期化
    for (int i = 0; i < PI_OUT_NUM; i++)
      digitalWrite(OUT_PIN_TO_PI_ARRAY[i], PI_OUT_OFF);
  }
  Serial.println("loop");
}

//------------------------------
//シリアル出力
//------------------------------

//sendModePCでのシリアルプリントに対する出力
void sendModePC_Print()
{
  char buff[50];
  sprintf(buff, "sendPC %4d, ", serialCount);
  Serial.print(buff);

  Serial.print("add: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sprintf(buff, "0x%2x ", gSensor[i].getAddress());
    Serial.print(buff);
  }

  Serial.print("tofRaw: ");
  for (int i = 0; i < SENSOR_NUM; i++)
  {
    sprintf(buff, "%4d, ", tofInitRaw[i]);
    Serial.print(buff);
  }

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


  Serial.println();

  delay(10);
}
