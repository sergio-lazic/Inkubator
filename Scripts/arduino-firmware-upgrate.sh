#!/bin/bash

cd /home/pi/Documents/Inkubator
mkdir arduinoFiles
cd arduinoFiles

ino init

cp /home/pi/Documents/Inkubator/ArduinoInkubator/ArduinoInkubator.ino /home/pi/Documents/Inkubator/arduinoFiles/src/sketch.ino
cp /home/pi/Documents/Inkubator/ArduinoInkubator/. /home/pi/Documents/Inkubator/arduinoFiles/src/ -R
rm /home/pi/Documents/Inkubator/arduinoFiles/src/ArduinoInkubator.ino

ino build -m mega2560
ino upload -m mega2560 -p /dev/ttyUSB0
ino clean

cd ..
rm -r -f arduinoFiles
