#include "Menu.h"

dataMenu::dataMenu(String name, float temperature, float humidity){
	_name = name;
	_temperature = temperature;
	_humidity = humidity;
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