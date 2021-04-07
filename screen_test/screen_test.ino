#include <DS3231.h>
#include <LCD_I2C.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int buttonState = 0; 
int previousButtonState = 0;
int buttonPin = D5;
int displayMode = 1;
int displayModeAmount = 3;
long startTime;
long stopTime;
bool h12Flag;
bool pmFlag;
bool century = true;
const char* ssid       = "TP-Link_CD38";
const char* password   = "16476523";
const long utcOffsetInSeconds = 7200;
LCD_I2C lcd(0x27);
DS3231 myClock;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(19200);
  
  if(connectToWifi()){
    setTime();
  }
  
  startTime = millis();
  lcd.begin();
  lcd.backlight(); 
  pinMode(buttonPin, INPUT);
  updateDisplay();
  buttonState = digitalRead(buttonPin);
}

bool connectToWifi() {
  WiFi.begin(ssid, password);

  for(int i = 0; i < 5; i++) {
    delay ( 1000 );
    Serial.print ( "." );
  }

  return WiFi.status() == WL_CONNECTED;
}

void setTime(){
  timeClient.begin();
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);

  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);

  clock.setYear(currentYear);
  clock.setMonth(currentMonth > 9 ? currentMonth : "0" + currentMonth);
  clock.setDate(monthDay > 9 ? monthDay : "0" + monthDay);
  clock.setDoW(timeClient.getDay());
  clock.setHour("21");
  clock.setMinute("42");
  clock.setSecond("22");
//  timeClient.update();
//  Serial.println(timeClient.getFormattedTime());
}

void loop() {
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
  float temp = myClock.getTemperature();
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(("Date & Time"));
  lcd.setCursor(0, 1);
  lcd.print(myClock.getYear());
  lcd.print('/');
  lcd.print(myClock.getMonth(century));
  lcd.print('/');
  lcd.print(myClock.getDate());
  lcd.print(' ');
  lcd.print(myClock.getHour(h12Flag, pmFlag));
  lcd.print(':');
  lcd.print(myClock.getMinute());
}

void customMessageDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
  lcd.setCursor(0, 1);
  lcd.print("This is fun");
}
