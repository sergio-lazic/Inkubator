#include "Button.h"        //https://github.com/JChristensen/Button
#include "DHT.h"
#include <LiquidCrystal.h>


#define DN_PIN 25          //Connect two tactile button switches (or something similar)
#define UP_PIN 29          //from Arduino pin 2 to ground and from pin 3 to ground.
#define OK_PIN 27
#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
				  //switch is closed, this is negative logic, i.e. a high state
				  //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.

#define REPEAT_FIRST 500   //ms required before repeating on long press
#define REPEAT_INCR 100    //repeat interval for long press
#define MIN_COUNT 0
#define MAX_COUNT 59

#define DHTPIN 2
#define DHTTYPE DHT21

LiquidCrystal lcd (A0, A1, A2, A3, A4, A5);
Button btnUP(UP_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the buttons
Button btnDN(DN_PIN, PULLUP, INVERT, DEBOUNCE_MS);
Button btnOK(OK_PIN, PULLUP, INVERT, DEBOUNCE_MS);

DHT dht(DHTPIN, DHTTYPE);

enum {WAIT, INCR, DECR};              //The possible states for the state machine
uint8_t STATE;                        //The current state machine state
int count;                            //The number that is adjusted
int lastCount = -1;                   //Previous value of count (initialized to ensure it's different when the sketch starts)
unsigned long rpt = REPEAT_FIRST;     //A variable time that is used to drive the repeats for long presses

void setup(void){
	lcd.begin(16, 2);
	lcd.clear();
	lcd.print("Hello bitchez!");
	lcd.setCursor(0, 1);
	lcd.print("lawl");
	pinMode(13, OUTPUT);
	Serial.begin(115200);

	dht.begin();
}

void loop(void){
	if(Serial.available()) stringHandler(Serial.readStringUntil('\n'));
	btnUP.read();                             //read the buttons
	btnDN.read();

	if (count != lastCount) {                 //print the count if it has changed
		lastCount = count;
		Serial.println(count, DEC);
	}

	switch (STATE) {
	case WAIT:                                //wait for a button event
	if (btnUP.wasPressed())
		STATE = INCR;
	else if (btnDN.wasPressed())
		STATE = DECR;
	else if (btnUP.wasReleased())         //reset the long press interval
		rpt = REPEAT_FIRST;
	else if (btnDN.wasReleased())
		rpt = REPEAT_FIRST;
	else if (btnUP.pressedFor(rpt)) {     //check for long press
		rpt += REPEAT_INCR;               //increment the long press interval
		STATE = INCR;
	}
	else if (btnDN.pressedFor(rpt)) {
		rpt += REPEAT_INCR;
		STATE = DECR;
	}
	break;

	case INCR:                                //increment the counter
		count = min(count++, MAX_COUNT);      //but not more than the specified maximum
		STATE = WAIT;
	break;

	case DECR:                                //decrement the counter
		count = max(count--, MIN_COUNT);      //but not less than the specified minimum
		STATE = WAIT;
		break;
	}
}

void stringHandler(String data){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(data);
}

void readSenosrs(){
	Serial.print(dht.readHumidity()); Serial.print("%,");
	Serial.print(dht.readTemperature()); Serial.println("*C");
}

