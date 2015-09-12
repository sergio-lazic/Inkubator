#!/usr/bin/python

import serial
from time import sleep
ser = serial.Serial("/dev/ttyACM0", 115200)
ser.flushInput(); ser.flushOutput()
sleep(0.1)
ser.write("data\n")
