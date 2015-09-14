#!/usr/bin/python
import json, serial, time
from datetime import datetime

info = "/home/pi/Documents/Inkubator/Scripts/data.json"
vrsta = "empty"
temp = 0;
vlag = 0;
dan = 0;
status=0;
meniFunc=0

menuLayer = 0
menuIndex = 0

dInfo={"vrsta":"empty","temp":0,"vlag":0}

arduino = serial.Serial("/dev/ttyACM0", 115200)
arduino.flushInput(); arduino.flushOutput()

com =":"
setCursor="sc"
writeText="lcd"
readSensor="rs"
ventOn="f"
ventOff="f"
heater="h"

file = open(info, 'r')
data = file.read()
file.close()
data = json.loads(data)

#cuuuzz meni

def toLCD(x, y, data):
	arduino.flushInput(); arduino.flushOutput()
	arduino.write(setCursor+com+str(x)+","+str(y)+"\n"+writeText+com+data+"\n")
	print setCursor+com+str(x)+","+str(y)
	print writeText+com+data

def btnHandler(inputData):
	global menuLayer
	global menuIndex
	print(inputData)
	if "OKL" in inputData and menuLayer == 0:
		menuLayer = 1
		toLCD(0,0,"       LOL      ")
	if "UP" in inputData and menuLayer == 1:
		menuIndex = menuIndex + 1
		toLCD(0,0,str(menuIndex)+" ")
	if "DN" in inputData and menuLayer == 1:
		menuIndex = menuIndex - 1
		toLCD(0,0,str(menuIndex)+" ")

def menuDisplay():
	global status, data
	arduino.write(setCursor+com+"0,0\n")
	arduino.write(writeText+com+"Vrsta:\n")
	arduino.write(setCursor+com+"1,0\n")
	arduino.write(writeText+com+dInfo.vrsta)

def meniUp():
	global status
	if status>=len(data):
		status=0
	meniFunc.display()

def meniDown():
	global status
	if status<0:
		status=len(data)-1
	meniFunc.display()

def meniOk():
	dInfo=data[status]
	meniFunc=setMeni
	meniFunc.display()

mainMeni={"display": menuDisplay, "meniUp":meniUp, "meniDown":meniDown, "meniOk":meniOk, "meniOkl":meniOk, "name":"main"}

#set meni

def setDisplay():
	arduino.write(setCursor+com+"0,0\n")
	arduino.write(writeText+com+dInfo.vrsta)
	arduino.write(setCursor+com+"> set temp: "+str(dInfo.temp))
def setUp():
	dInfo.temp+=1
	meniFunc.display()

def setDown():
	dInfo.temp-=1
	meniFunc.display()

def setOk():
	meniFunc=statusMeni
	meniFunc.display()

setMeni={"display": setDisplay, "meniUp":setUp, "meniDown":setDown, "meniOk":setOk, "meniOkl":setOk, "name":"set"}
#status meni

def statusOk():
	meniFunc=mainMeni
	meniFunc.display()

def statusDisplay():
	arduino.write(setCursor+com+"0,0\n")		#print wrsta
	arduino.write(writeText+com+"vrsta"+"\n")
	if dan>9:									#print dani inkubacije
		arduino.write(setCursor+com+"0,14\n")
	else:
		arduino.write(setCursor+com+"0,15\n")
	arduino.write(writeText+com+str(dan)+"\n")

	arduino.write(setCursor+com+"1,1\n")			#print temp
	arduino.write(writeText+com+"T:"+str(temp)+"\n")
	arduino.write(setCursor+com+"1,6\n")			#print vlaga
	arduino.write(writeText+com+"V:"+str(vlag)+"\n")

def dummy():
	pass

statusMeni={"display":statusDisplay, "meniUp":dummy, "meniDown":dummy, "meniOk":dummy, "meniOkl":statusOk, "name":"status"}

#######

didiIt = 0
def meni():
	global meniFunc
	global didiIt
	if arduino.inWaiting():
		inData=arduino.readline().translate(None,"\n\r")
		if "btn" in inData: btnHandler(inData[3:])
		else:
			print datetime.now().strftime("%y-%m-%d %h:%M:%S "),
			print inData

	if int(datetime.now().strftime("%S"))%10 == 0:
		if didiIt == 0:
			arduino.write("rs\n")
		didiIt = 1
	else: didiIt = 0
	#datetime.now().strftime("sec 0,year %Y,month %m,day %d,hour %H,min %M,sec %S")

meniFunc=statusMeni
while 1:
	meni()
	time.sleep(0.1)