/* 
THE AUTOMATIC COVER - SENSOR EDITION

Ports:
Pressure - Analog pin A0
Humidity - Digital pin 8 (3.3v)
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

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Auto Cover");
  lcd.setCursor(0,1);
  lcd.print("Started");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {
	
// CHECK PRESSURE

 int pressure = analogRead(A0);
 delay(500);
 
 // IF PRESSURE DETECTED, CHECK HUMIDITY
 
 if (pressure > 50) {
 	Serial.println("PRESSURE DETECTED");
 	lcd.clear();
 	lcd.setCursor(0,0);
 	lcd.print("Rain drop");
 	lcd.setCursor(0,1);
 	lcd.print("detected");
 	delay(1000);
 	lcd.clear();
 	lcd.setCursor(0,0);
 	lcd.print("Checking");
 	lcd.setCursor(0,1);
 	lcd.print("humidity");
 	delay(3000);
 	float humidity = dht.readHumidity();
 	if (humidity > 50) {
 		
 // IF PRESSURE DETECTED AND HUMIDITY > 90%, isRaining = true
 
 		bool isRaining = true;
 		Serial.println("RAIN DETECTED");
 		lcd.clear();
 		lcd.print("Rain");
 		lcd.setCursor(0,1);
 		lcd.print("detected");
 		delay(10000);
 	}
 	else {
 		Serial.print("FALSE POSITIVE - HUMIDITY OVERRIDE");
 		lcd.clear();
 		lcd.setCursor(0,0);
 		lcd.print("Humidity");
 		lcd.setCursor(0,1);
 		lcd.print("Override");
 		delay(10000);
		}
	}
}