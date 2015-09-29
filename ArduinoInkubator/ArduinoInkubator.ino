#include "Button.h"        //https://github.com/JChristensen/Button
#include "DHT.h"
#include <LiquidCrystal.h>

#define DN_PIN 7          //Connect two tactile button switches (or something similar)
#define UP_PIN 5          //from Arduino pin 2 to ground and from pin 3 to ground.
#define OK_PIN 6
#define BL_PIN 5
#define HTR_PIN 9
#define FAN_PIN 10
#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
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

String nameArray[]{
  " Leopard Gekon  ",
  " Crested Gekon  ",
  " Bradata Agama  "
};
float tempArray[]{
  28, 25, 27
};

float currentTemperature, currentHumidity;

int menuItmOld = 0, menuItm = 0, menuLvl = 0;
enum {WAIT, INCR, DECR};              //The possible states for the state machine
uint8_t STATE;                        //The current state machine state
int count;                            //The number that is adjusted
int lastCount = -1;                   //Previous value of count (initialized to ensure it's different when the sketch starts)
unsigned long rpt = REPEAT_FIRST;     //A variable time that is used to drive the repeats for long presses
bool okFlag = false;
int longPress = 500;
void setup(void){
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(" smartInkubator ");
  pinMode(13, OUTPUT);
  pinMode(BL_PIN, OUTPUT);
  pinMode(HTR_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  Serial.begin(115200);
  dht.begin();
}

void loop(void){
  if(Serial.available()) serialHandler();
  btnUP.read(); btnDN.read(); btnOK.read();

       if (btnUP.wasReleased()){ Serial.println("btnUP"); if(menuLvl) menuItm ++; }
  else if (btnDN.wasReleased()){ Serial.println("btnDN"); if(menuLvl) menuItm --; }
  else if (btnUP.pressedFor(longPress*2)){ Serial.println("btnUPL"); btnUP.reset(); }
  else if (btnDN.pressedFor(longPress*2)){ Serial.println("btnDNL"); btnDN.reset(); }
  else if (btnOK.pressedFor(longPress*2)){
    menuLvl = 1; Serial.println("btnOKL"); okFlag= true; btnOK.reset(); changeGekon();
  }
  else if (btnOK.wasReleased()){
    if(okFlag) okFlag= false;
    if(menuLvl) menuLvl=0;
    else Serial.println(menuLvl);
  }

  if (menuItm != menuItmOld){
    changeGekon();
  }

  if (menuLvl == 0){
    readDHT();
    sensorDataToLDC();
    termostat(tempArray[menuItm]);
  }
}

void serialHandler(){
  String data = Serial.readStringUntil('\n');
  if(data.startsWith("sc")) lcd.setCursor(data.substring(data.indexOf(':')+1, data.indexOf(',')).toInt(), data.substring(data.indexOf(',')+1, data.length()).toInt());
  else if(data.startsWith("h")) digitalWrite(HTR_PIN, data.substring(data.indexOf(':')+1, data.length()).toInt());
  else if(data.startsWith("f")) digitalWrite(FAN_PIN, data.substring(data.indexOf(':')+1, data.length()).toInt());
  else if(data.startsWith("bl")) analogWrite(data.substring(data.indexOf(':')+1, data.length()).toInt(), BL_PIN);
  else if(data.startsWith("lcd")) lcd.print(data.substring(data.indexOf(':')+1, data.length()));
  else if(data.startsWith("rs")) sendSensorValue();
  else Serial.println(data);

}

void sendSensorValue(){
  Serial.print(currentTemperature,1);
  Serial.print(",");
  Serial.print(currentHumidity,1);
  Serial.print(",");
  Serial.print(digitalRead(HTR_PIN));
  Serial.print(",");
  Serial.println(digitalRead(FAN_PIN));
}

void sensorDataToLDC(){
  lcd.setCursor(0,1);
  lcd.print(currentTemperature, 1);
  lcd.print("C  hum=");
  lcd.print(currentHumidity, 1);
  lcd.print("%");
}

void termostat(float temp){
  if (currentTemperature >= temp+0.1) digitalWrite(HTR_PIN, LOW);
  if (currentTemperature <= temp-0.1) digitalWrite(HTR_PIN, HIGH);
}
void readDHT(){
  currentTemperature = dht.readTemperature();
  currentHumidity = dht.readHumidity();
}

void display(){
  lcd.clear();
  delay(10);
  lcd.println(nameArray[menuItm]);
  lcd.setCursor(0,1);
  lcd.print(tempArray[menuItm], 1);
  lcd.print("C  hum=");
  lcd.print(50, 1);
  lcd.print("%");
}

void changeGekon(){
  if (menuItm == -1) menuItm = 2;
  if (menuItm == 3) menuItm = 0;
  display();
  menuItmOld = menuItm;
}


