#!/usr/bin/python
import serial, subprocess
from datetime import datetime
from time import sleep

arduino = serial.Serial("/dev/ttyUSB0", 115200)

def arduinoSend(data):
  arduino.write(data)
  sleep(0.05)

hour = datetime.now().strftime("%H")
minute = datetime.now().strftime("%M")
second = datetime.now().strftime("%S")
string = hour + ':' + minute + ":" + second
while len(string) < 20: string = ' ' + string
string = '23' + string + '\n'
arduinoSend(string)

print "bok!"
