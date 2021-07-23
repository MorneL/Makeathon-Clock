#include <RTClib.h>
#include <LCD_I2C.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "WifiCredentials.h"

const long UTC_OFFSET_S = 7200;
const long DURATION_MS = 3000;
const long SPEED = 19200;
const int DISPLAY_COUNT = 3;

LCD_I2C lcd(0x27);
RTC_DS3231 rtcClock;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_S);

int currentDisplay = 0;
int buttonState = 0;
int previousButtonState = 0;
int buttonPin = D5;

long startTime = millis();
long stopTime;
String currentDisplayContent[2];
String newDisplayContent[2];

void (*getDisplayContents[DISPLAY_COUNT])() {
  displayTemp,
  displayTime,
  displayCustomMsg
};

bool connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  for (int i = 0; i < 5; i++) {
    delay(1000);
  }

  return WiFi.status() == WL_CONNECTED;
}

void setTime() {
  timeClient.begin();
  timeClient.update();

  long epochTime = timeClient.getEpochTime();
  rtcClock.adjust(DateTime(epochTime));
}

void updateDisplay() {
  (*getDisplayContents[currentDisplay])();

  if (!arrayEqual(currentDisplayContent, newDisplayContent)) {
    copyArray(currentDisplayContent, newDisplayContent);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(currentDisplayContent[0]);

    lcd.setCursor(0, 1);
    lcd.print(currentDisplayContent[1]);
  }
}

bool arrayEqual(String a[], String b[]) {
  for (int i = 0; i < 2; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

void copyArray(String a[], String b[]) {
  for (int i = 0; i < 2; i++) {
    a[i] = b[i];
  }
}

void displayTemp() {
  float temp = rtcClock.getTemperature();

  if (isnan(temp)) {
    newDisplayContent[0] = "Error, check";
    newDisplayContent[1] = "temp sensor";

    return;
  }

  newDisplayContent[0] = "Temp: " + (String)temp;
  newDisplayContent[1] = "";
}

void displayTime() {
  DateTime datetime = rtcClock.now();

  newDisplayContent[0] = "Date & Time";
  newDisplayContent[1] =
    (String)datetime.year()
    + "/" + (String)datetime.month()
    + "/" + (String)datetime.day()
    + " " + (String)datetime.hour()
    + ":" + (String)datetime.minute();
}

void displayCustomMsg() {
  newDisplayContent[0] = "#ct-makeathon";
  newDisplayContent[1] = "Coding! 🚀👨‍💻👩‍💻";
}

void setup() {
  Serial.begin(SPEED);
  rtcClock.begin();

  if (connectToWifi()) {
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
    if (currentDisplay < DISPLAY_COUNT - 1) {
      currentDisplay++;
    } else {
      currentDisplay = 0;
    }

    updateDisplay();
  }

  stopTime = millis();

  if (stopTime - startTime > DURATION_MS) {
    updateDisplay();
    stopTime = startTime;
    startTime = millis();
  }
}
