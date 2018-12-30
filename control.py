import serial
import sys
import time

ser = serial.Serial()
ser.port = sys.argv[1]
ser.baudrate = 9600
ser.setDTR(False)
ser.open()
time.sleep(2)
ser.write(" ".join(sys.argv[2:]).encode("ascii") + b"\n")
print(ser.readline())
print(ser.readline())
ser.close()