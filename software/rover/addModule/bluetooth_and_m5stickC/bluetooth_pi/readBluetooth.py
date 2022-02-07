import serial
import struct

ser=serial.Serial('/dev/rfcomm1')

while 1:
    data = ser.read()
    value = struct.unpack('B', data)
    print(value)
