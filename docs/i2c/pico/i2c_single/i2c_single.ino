/* This example shows how to get single-shot range
  measurements from the VL53L0X. The sensor can optionally be
  configured with different ranging profiles, as described in
  the VL53L0X API user manual, to get better performance for
  a certain application. This code is based on the four
  "SingleRanging" examples in the VL53L0X API.

  The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>


#define I2C_DATA_SIZE 0x0F
#define I2C_ADDRESS 0x08

byte i2cData[I2C_DATA_SIZE];
byte i2cNoElement = 0;


VL53L0X sensor;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY


void setup()
{
  Serial.begin(9600);
  Wire.setSDA(0);
  Wire.setSCL(1);

  Wire1.setSDA(2);
  Wire1.setSCL(3);

  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif

  for (int i = 0; i < I2C_DATA_SIZE; i++) {
    i2cData[i] = i;
  }

  Wire1.begin(I2C_ADDRESS);// Slave ID #8

  Wire1.onRequest(requestEvent);
  Wire1.onReceive(receiveEvent);

}

void loop()
{

  for (int i = 0; i < I2C_DATA_SIZE; i++) {
    Serial.print("0x");
    Serial.print(i2cData[i], HEX);
    Serial.print(", ");
  }
  //Serial.println();
  int a = sensor.readRangeSingleMillimeters();
  i2cData[0xf - 1] = a / 0xff;
  Serial.print(a / 0xff, HEX);
  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  Serial.println();
  delay(100);
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
