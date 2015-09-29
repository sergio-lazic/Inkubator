#include "Menu.h"

dataMenu::dataMenu(String theName, float temperature, float humidity){
	_name = theName;
	_temperature = temperature;
	_humidity = humidity;
}

dataMenu::dataMenu(const dataMenu &dM){
	this-> _name = dM._name;
	this-> _temperature = dM._temperature;
	this-> _humidity = dM._humidity;
}
dataMenu::dataMenu(){
	this-> _name = " smartInkubator ";
	this-> _temperature = 0;
	this-> _humidity = 0;
}


String dataMenu::name(){
	return _name;
}

float dataMenu::temperature(){
	return _temperature;
}

float dataMenu::humidity(){
	return _humidity;
}

void dataMenu::operator=(const dataMenu &dM){
	this-> _name = dM._name;
	this-> _temperature = dM._temperature;
	this-> _humidity = dM._humidity;
}