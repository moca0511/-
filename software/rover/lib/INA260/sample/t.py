import time
import board
import adafruit_ina260

i2c = board.I2C()
ina260 = adafruit_ina260.INA260(i2c)
ina260_1 = adafruit_ina260.INA260(i2c, 0x44)
while True:
    print("%6.2fmA %.2fV %7.2fw,  %6.2fmA %.2fV %7.2fw,  "
          % (ina260.current, ina260.voltage, ina260.power, ina260_1.current, ina260_1.voltage, ina260_1.power))
    time.sleep(1)
