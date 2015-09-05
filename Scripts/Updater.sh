#!/bin/bash

var=$(git pull)
echo $var
if [[ $var == *"ArduinoInkubator"* ]]; then
	/home/pi/Documents/Inkubator/Scripts/arduino-firmware-upgrate.sh
fi
