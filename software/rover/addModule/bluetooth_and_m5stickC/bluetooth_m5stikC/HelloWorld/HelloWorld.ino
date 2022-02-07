//ボタンについて
//https://lang-ship.com/reference/unofficial/M5StickC/Class/Button/

//display
//https://lang-ship.com/reference/unofficial/M5StickC/Tips/M5Display/
//https://lang-ship.com/reference/unofficial/M5StickC/Class/M5Display/

#include <M5StickC.h>
//#include "efontEnableJa.h"
//#include "efont.h"
#include "BluetoothSerial.h"
#include "AXP192.h"
BluetoothSerial bts;
unsigned long previousMillis;

// the setup routine runs once when M5StickC starts up
void setup()
{
  // Initialize the M5StickC object
  M5.begin();
  M5.Lcd.setRotation(1);

  M5.Axp.ScreenBreath(9); //ディスプレイの明るさ(最大12)

  // LCD display
  M5.Lcd.print("Hello World");

  //M5.Lcd.setTextSize(2);

  Serial.begin(9600);
  bts.begin("M5StackC"); //PC側で確認するときの名前
  previousMillis = millis();
}

#define CONECT_CHECK_DATA 201

int btnA, btnB, count, btnB_long, btnB_long_before, sendData;
// the loop routine runs over and over again forever
void loop()
{
  M5.update();
  btnA = M5.BtnA.isPressed();
  btnB = M5.BtnB.wasReleased();
  btnB_long = M5.BtnB.pressedFor(500);

  if (btnB == 1 && btnB_long_before != 1)
    count++;

  if (count >= 100)
    count = 0;

  if (btnB_long_before == 1 && btnB_long == 0)
    count = 0;
  btnB_long_before = btnB_long;

  if (btnA == 1)
    sendData = 100 + count;
  else
    sendData = count;

  M5.Lcd.setCursor(10 * 8, 0);
  M5.Lcd.setTextFont(0);
  M5.Lcd.printf("batV= %.3f  \n", M5.Axp.GetBatVoltage()); //バッテリーは4.2Vまで充電して、3.0V以下になったらシャットダウンします。

  M5.Lcd.setTextFont(1);
  M5.Lcd.setCursor(0, 9);
  M5.Lcd.printf("bttnA=%d, bttnB=%d, long=%d \n", btnA, btnB, btnB_long);
  M5.Lcd.printf("count=%3d, sData=%3d \n", count, sendData);

  bts.write(sendData);

  int receiveData;
  if (bts.available())
  {
    previousMillis = millis();
    receiveData = bts.read();
    if (receiveData != CONECT_CHECK_DATA)
      M5.Lcd.printf("new    get = %3d \n", receiveData);
    else
      M5.Lcd.printf("check  \n", receiveData);
  }
  else
  {
    M5.Lcd.printf("      \n");
  }

  M5.Lcd.setTextFont(0);
  if (millis() - previousMillis > 5 * 1000)
    M5.Lcd.printf("connect err?\n");
  else
  {
    M5.Lcd.printf("                    \n\n");
    M5.Lcd.printf(" 0=null,1=stop\n");
    M5.Lcd.printf(" 2=P, 3=B, 4=R, 5=L\n");
    M5.Lcd.printf("batVrange=3.0~4.2V\n");
  }
  delay(100);
}
