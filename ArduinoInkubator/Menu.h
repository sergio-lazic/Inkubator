#ifndef Menu_h
#define Menu_h
#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

class dataMenu
{
public:
	dataMenu(String Name, float temperature, float humidity);

//private:
	String Name;
	float temperature;
	float humidity;
};
#endif