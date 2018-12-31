import serial
import sys
import time
import struct

ser = serial.Serial()
ser.port = sys.argv[1]
ser.baudrate = 115200
ser.open()
time.sleep(2)
while True:
  line = sys.stdin.readline().upper()
  if line == "EXIT\n":
    break
  arr = line.split(" ")
  bytesToWrite = arr[0].encode("ascii") + b" "
  if len(arr) > 1:
    for i in range(1, len(arr)):
      bytesToWrite += struct.pack("B", int(arr[i]))
  bytesToWrite += b"\n"
  ser.write(bytesToWrite)
ser.close()