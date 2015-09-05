#!/bin/bash

cd /home/pi/Documents/Inkubator/

var=$(git pull)



echo $var
if [[ $var == *"ArduinoInkubator"* ]]; then
	Scripts/arduino-firmware-upgrate.sh
fi
