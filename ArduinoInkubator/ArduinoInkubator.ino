#include <LiquidCrystal.h>

LiquidCrystal lcd(32,30,28,26,24,22);
/*
    *** KONFIGURACIJA LCD-A S ARDUINO MEGA2560 PLOCOM ***
      LCD RS pin ARDUINO MEGA pin 32
      LCD E pin ARDUINO MEGA pin 30
      LCD D4 pin  ARDUINO MEGA pin 28
      LCD D5 pin ARDUINO MEGA pin 26
      LCD D6 pin ARDUINO MEGA pin 24
      LCD D7 pin ARDUINO MEGA pin 22
*/

byte customChars[][8] = {
  {
    B10000,
    B11000,
    B11100,
    B11110,
    B11100,
    B11000,
    B10000,
    B00000
  },
  {
    B00001,
    B00011,
    B00111,
    B01111,
    B00111,
    B00011,
    B00001,
    B00000
  },
  {
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B00000,
    B00000
  },
  {
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
  },
  {
    B00000,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110,
    B01110,
    B00000
  },
  {
    B11111,
    B10001,
    B10001,
    B00000,
    B00000,
    B10001,
    B10001,
    B11111
  },
  {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  },
  {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  }
};

int brojcanikS = 0;
int brojcanikSo = brojcanikS;
int enable = 0;
unsigned long okPresedTime;
unsigned long okStartTime;

int bl = 25;
int bd = 29;
int bo = 27;

bool bds, bdso;
bool bls, blso;
bool bos, boso;

int okEdge = -1;
bool okEdgeOld = 1;

int brojStranica = 5;
int killEnable = 2;

String stranica[10]{
  " Prva stranica  ",
  " podatak 1      ",
  " podatak 2      ",
  " podatak 3      ",
  " podatak 4      ",
};

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  for(byte i=0; i<8; i++) lcd.createChar(i, customChars[i]);
  
  pinMode(13, OUTPUT);
  printGornjiRed(brojcanikS);

  pinMode(bl, INPUT_PULLUP);
  pinMode(bd, INPUT_PULLUP);
  pinMode(bo, INPUT_PULLUP);

  bds = digitalRead(bd); bdso = bds;
  bls = digitalRead(bl); blso = bls;
  bos = digitalRead(bo); boso = bos;
}

void loop(){
  bds = digitalRead(bd);
  bls = digitalRead(bl);
  bos = digitalRead(bo);

  digitalWrite(13, enable);

  if(okEdge == 0){
    okPresedTime = millis() - okStartTime;
    if(okPresedTime > 1000 && enable == 0) enable = 2;
  }

  if(bos != boso){
    delay(10);
    if(bos != boso){
      if(bos == LOW){
        okEdge = 0;
        okStartTime = millis();
        if(enable) enable --;
      }
      else {
        okEdge = 1;
      }

      if(enable == 2){
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
        lcd.setCursor(0, 1);
        lcd.write(byte(1));
      }

      else{
        lcd.setCursor(15, 1);
        lcd.write(" ");
        lcd.setCursor(0, 1);
        lcd.write(" ");
      }
      boso = bos;
    }
  }

  if(bds != bdso){
    delay(10);
    if(bds != bdso){
      if(bds == LOW){
        if(enable == 2){
          brojcanikS ++;
          if(brojcanikS == brojStranica) brojcanikS = 0;
        }
      }
      bdso = bds;
    }
  }

  if(bls != blso){
    delay(10);
    if(bls != blso){
      if(bls == LOW){
        if(enable == 2){
          brojcanikS --;
          if(brojcanikS == -1) brojcanikS = brojStranica-1;
        }
      }
      blso = bls;
    }
  }

  if(brojcanikS != brojcanikSo){
    printGornjiRed(brojcanikS);
    brojcanikSo = brojcanikS;
  }
}

void printGornjiRed(byte i){
  lcd.setCursor(0,0);
  lcd.print(stranica[i]);
}
