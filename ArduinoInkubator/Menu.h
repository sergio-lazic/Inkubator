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
	dataMenu(String name, float temperature, float humidity);
	String name();
	float temperature();
	float humidity();

private:
	String _name;
	float _temperature;
	float _humidity;
};
#endif