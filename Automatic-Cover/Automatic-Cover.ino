/*
THE AUTOMATIC COVER - SENSOR EDITION

Ports:
Pressure - Analog pin A0
Humidity (DHT22) - Digital pin 8 (3.3v)
LCD - Digital pins 2, 3, 4, 5, 11, and 12

NEEDED LIBARIES:
DHT Sensor Libary: https://github.com/adafruit/DHT-sensor-library

TO-DO

1. Check if it's still raining every 10 minutes
*/

// INITIALIZE HUMIDITY SENSOR
#include "DHT.h"
#define DHTPIN 8     // Humidity digital pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.

// INITIALIZE LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// INITIALIZE VARIABLES
bool isRaining;
int pressure;
float humidity;
unsigned long previousMillis = 0;
const long interval = 60000; // Rain update frequency (ms)

void checkHumidity() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Checking");
  lcd.setCursor(0,1);
  lcd.print("humidity");
  delay(3000);
  float humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (humidity > 50) {

  // IF PRESSURE DETECTED AND HUMIDITY > 90%, isRaining = true

    bool isRaining = true;
    Serial.print("Rain Detected");
    Serial.println("");
    lcd.clear();
    lcd.print("Rain");
    lcd.setCursor(0,1);
    lcd.print("detected");
    delay(10000);
  }
  else {

    // IF PRESSURE DETECTED AND HUMIDITY < 90%, isRaining = False

    bool isRaining = false;
    Serial.print("False positive - humidity override");
    Serial.println("");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity");
    lcd.setCursor(0,1);
    lcd.print("Override");
    delay(10000);
    }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Auto Cover");
  lcd.setCursor(0,1);
  lcd.print("Started");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {

// CHECK PRESSURE

 int pressure = analogRead(A0);
 delay(100);

 // IF PRESSURE DETECTED, CHECK HUMIDITY

 if (pressure > 50) {
  Serial.print("Pressure: ");
  Serial.println(pressure);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rain drop");
  lcd.setCursor(0,1);
  lcd.print("detected");
  delay(1000);
  checkHumidity();
  }

// RAIN UPDATING

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  if (isRaining = true) {
    checkHumidity();
    }
  }
}
