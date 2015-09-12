#!/bin/bash

cd /home/pi/Documents/Inkubator
mkdir arduinoFiles
cd arduinoFiles

ino init

cp /home/pi/Documents/Inkubator/ArduinoInkubator/ArduinoInkubator.ino /home/pi/Documents/Inkubator/arduinoFiles/src/sketch.ino
cp /home/pi/Documents/Inkubator/ArduinoInkubator/. /home/pi/Documents/Inkubator/arduinoFiles/src/ -R
rm /home/pi/Documents/Inkubator/arduinoFiles/src/ArduinoInkubator.ino

ino build -m leonardo
ino upload -m leonardo -p /dev/ttyACM0
ino clean

cd ..
rm -r -f arduinoFiles
