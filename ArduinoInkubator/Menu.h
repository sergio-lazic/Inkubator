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
	dataMenu();
	dataMenu(const dataMenu &dM);
	dataMenu(String theName, float temperature, float humidity);
	
	void operator=(const dataMenu &dM);

	String name();
	float temperature();
	float humidity();

private:
	String _name;
	float _temperature;
	float _humidity;
};
#endif