#!/usr/bin/python
import json, serial, time

info = "/home/pi/Documents/Inkubator/Scripts/data.json"
vrsta = "empty"
temp = 0;
vlag = 0;
dan = 0;
status=0;
meniFunc=0

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

mainMeni={"display": menuDisplay, "meniUp":meniUp, "meniDown":meniDown, "meniOk":meniOk, "meniOkl":meniOk, "namez":"main"}

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

setMeni={"display": setDisplay, "meniUp":setUp, "meniDown":setDown, "meniOk":setOk, "meniOkl":setOk, "namez":"set"}
#status meni

def statusOk():
	meniFunc=mainMeni
	meniFunc.display()

def statusDisplay():
	arduino.write(setCursor+com+"0,0\n")		#print wrsta
	arduino.write(writeText+com+"vrsta"+"\n")
	if dan>9:									#print dani inkubacije
		arduino.write(setCursor+com+"0,14")
	else:
		arduino.write(setCursor+com+"0,15")
	arduino.write(writeText+com+str(dan)+"\n")

	arduino.write(setCursor+com+"1,1")			#print temp
	arduino.write(writeText+com+"T:"+str(temp))
	arduino.write(setCursor+com+"1,6")			#print vlaga
	arduino.write(writeText+com+"V:"+str(vlag))

def dummy():
	pass

statusMeni={"display":statusDisplay, "meniUp":dummy, "meniDown":dummy, "meniOk":dummy, "meniOkl":statusOk, "namez":"status"}

#######

def meni():
	global meniFunc
	if arduino.inWaiting():
		inData=arduino.readline()
		print inData.split()
		if inData=="btnUP":
			meniFunc.meniUp()
		elif inData=="btnDN":
			meniFunc.meniDown()
		elif inData =="btnOK":
			meniFunc.ok()
		elif inData =="btnOKL":
			meniFunc.okl()

meniFunc=statusMeni
while 1:
	meni()
	time.sleep(0.1)
