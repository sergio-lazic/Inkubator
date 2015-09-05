#!/bin/bash

cd /home/pi/Documents/Inkubator
if [ ! -d "arduinoFIles" ]; then
	mkdir $DIRECTORY
fi

cd $DIRECTORY

if [ ! -d "src" ]; then
	ino init
fi

cp /home/pi/Documents/Inkubator/ArduinoInkubator/ArduinoInkubator.ino /home/pi/Documents/Inkubator/arduinoFIles/src/sketch.ino
cp /home/pi/Documents/Inkubator/ArduinoInkubator/. /home/pi/Documents/Inkubator/arduinoFIles/src/ -R
rm /home/pi/Documents/Inkubator/arduinoFiles/src/ArduinoInkubator.ino

ino build -m mega2560
ino upload -m mega2560 -p /dev/ttyUSB0
ino clean
