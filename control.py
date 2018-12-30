import serial
import sys
import time

ser = serial.Serial()
ser.port = sys.argv[1]
ser.baudrate = 9600
ser.setDTR(False)
ser.open()
time.sleep(2)
while True:
  line = sys.stdin.readline().upper()
  if line == "EXIT\n":
    break
  ser.write(line.encode("ascii"))
ser.close()