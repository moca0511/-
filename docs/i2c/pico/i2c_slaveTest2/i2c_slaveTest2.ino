#include <Wire.h>

#define I2C_DATA_SIZE 0x0F
#define I2C_ADDRESS 0x08

byte i2cData[I2C_DATA_SIZE];
byte i2cNoElement = 0;

void setup() {
  for (int i = 0; i < I2C_DATA_SIZE; i++) {
    i2cData[i] = i;
  }


  Serial.begin(9600);

  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin(I2C_ADDRESS);// Slave ID #8
  
  Wire1.onRequest(requestEvent);
  Wire1.onReceive(receiveEvent);
  
}

void loop() {
  for (int i = 0; i < I2C_DATA_SIZE; i++) {
    Serial.print("0x");
    Serial.print(i2cData[i], HEX);
    Serial.print(", ");
  }
  Serial.println();
}

void requestEvent() {
  Wire1.write(i2cData[i2cNoElement]);
}

void receiveEvent(int a) {
  i2cNoElement = Wire1.read();
  int i2cDataSize = Wire1.available();
  for (int i = 0; i < i2cDataSize; i++) {
    i2cData[i2cNoElement] = Wire1.read();
  }
}
