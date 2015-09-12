#!/usr/bin/python
import serial, subprocess
from datetime import datetime
from time import sleep

arduino = serial.Serial("/dev/ttyACM0", 115200)

def arduinoSend(data):
  arduino.write(data)
  sleep(0.05)

arduinoSend("string")
