#include <DS3231.h>
//#include <DHT.h>
#include "SerialTimeSet.h"
#include <LCD_I2C.h>


//#define DHTPIN 13 
//#define DHTTYPE DHT11

LCD_I2C lcd(0x27);
RTClib myRTC;
DS3231 clock;
SerialTimeSet timeSet;
//DHT dht(DHTPIN, DHTTYPE);
int buttonState = 0; 
int previousButtonState = 0;
int buttonPin = 2;
int displayMode = 1;
int displayModeAmount = 3;
long startTime;
long stopTime;

void setup()
{
    timeSet.setupFunction();
    startTime = millis();
    lcd.begin();
    lcd.backlight(); 
    pinMode(buttonPin, INPUT);
    updateDisplay();
    buttonState = digitalRead(buttonPin);
}

void loop()
{
  timeSet.loopFunction();
  previousButtonState = buttonState;
  buttonState = digitalRead(buttonPin);
  
  if (previousButtonState == HIGH && buttonState == LOW) {
    if(displayMode < displayModeAmount){
      displayMode++;
    } else {
      displayMode = 1;
    }

    updateDisplay();
  }  

  stopTime = millis();
  if(stopTime - startTime > 3000){
    updateDisplay();
    stopTime = startTime;
    startTime = millis();
  }
}

void updateDisplay(){
  switch(displayMode){
    case 1:
      tempDisplay();
      break;
    case 2:
      timeDisplay();
      break;
    case 3:
      customMessageDisplay();
      break;
    default:
      break;
  }
}

void tempDisplay(){
  float temp = clock.getTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  if (isnan(temp)) {
    lcd.print("Error, check");
    lcd.setCursor(0, 1);
    lcd.print("temp sensor");
  } else {
    lcd.print(("Temp: "));
    lcd.print(temp);
  }
}

void timeDisplay(){
  DateTime now = myRTC.now();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(("Date & Time"));
  lcd.setCursor(0, 1);
  lcd.print(now.year());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.day());
  lcd.print(' ');
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
}

void customMessageDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
  lcd.setCursor(0, 1);
  lcd.print("This is fun");
}
