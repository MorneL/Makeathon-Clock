#include <RTClib.h>
#include <LCD_I2C.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "WifiCredentials.h"

const long UTC_OFFSET_IN_SECONDS = 7200;
const int AMOUNT_OF_DISPLAYS = 3;

int currentDisplay = 0;
int buttonState = 0;
int previousButtonState = 0;
int buttonPin = D5;
long startTime = millis();
long stopTime;
String currentDisplayContent[2];
String newDisplayContent[2];

LCD_I2C lcd(0x27);
RTC_DS3231 rtcClock;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_IN_SECONDS);

void setup() {
  Serial.begin(19200);
  rtcClock.begin();

  if(connectToWifi()){
    setTime();
  }

  lcd.begin();
  lcd.backlight();
  pinMode(buttonPin, INPUT);
  updateDisplay();
  buttonState = digitalRead(buttonPin);
}

void loop() {
  previousButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (previousButtonState == HIGH && buttonState == LOW) {
    if (currentDisplay < AMOUNT_OF_DISPLAYS - 1){
      currentDisplay++;
    } else {
      currentDisplay = 0;
    }

    updateDisplay();
  }

  stopTime = millis();

  if (stopTime - startTime > 3000){
    updateDisplay();
    stopTime = startTime;
    startTime = millis();
  }
}

bool connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  for (int i = 0; i < 5; i++) {
    delay(1000);
  }

  return WiFi.status() == WL_CONNECTED;
}

void setTime(){
  timeClient.begin();
  timeClient.update();

  long epochTime = timeClient.getEpochTime();
  rtcClock.adjust(DateTime(epochTime));
}

void (*getDisplayContents[AMOUNT_OF_DISPLAYS])() {
  tempDisplay,
  timeDisplay,
  customMessageDisplay
};

void updateDisplay(){
  (*getDisplayContents[currentDisplay])();

  if (!arrayEqual(currentDisplayContent, newDisplayContent)){
    copyArray(currentDisplayContent, newDisplayContent);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(currentDisplayContent[0]);

    lcd.setCursor(0, 1);
    lcd.print(currentDisplayContent[1]);
  }
}

bool arrayEqual(String arr[], String secondArr[]){
  for (int i = 0; i < 2; i++){
    if (arr[i] != secondArr[i]){
      return false;
    }
  }
  return true;
}

void copyArray(String arr[], String newArr[]){
  for(int i = 0; i < 2; i++){
    arr[i] = newArr[i];
  }
}

void tempDisplay(){
  float temp = rtcClock.getTemperature();

  if (isnan(temp)) {
    newDisplayContent[0] = "Error, check";
    newDisplayContent[1] = "temp sensor";
  } else {
    newDisplayContent[0] = "Temp: " + (String)temp;
    newDisplayContent[1] = "";
  }
}

void timeDisplay(){
  DateTime rtcDateTime = rtcClock.now();

  newDisplayContent[0] = "Date & Time";
  newDisplayContent[1] = (String)rtcDateTime.year() + "/" + (String)rtcDateTime.month() + "/" + (String)rtcDateTime.day() + " " + (String)rtcDateTime.hour() + ":" + (String)rtcDateTime.minute();
}

void customMessageDisplay(){
  newDisplayContent[0] = "Hello World";
  newDisplayContent[1] = "This is fun";
}
