/* 
THE AUTOMATIC COVER - SENSOR EDITION

Ports:
Pressure - Analog pin A0
Humidity - Digital pin 2 (3.3v)
*/

#include "DHT.h"
#define DHTPIN 2     // Humidity digital pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.

void setup() {
  dht.begin();
  Serial.begin(9600);
}

void loop() {
	
// CHECK PRESSURE

 int pressure = analogRead(A0);
 delay(500);
 
 // IF PRESSURE DETECTED, CHECK HUMIDITY
 
 if (pressure > 50) {
 	Serial.println("PRESSURE DETECTED");
 	delay(1000);
 	float humidity = dht.readHumidity();
 	if (humidity > 50) {
 		
 // IF PRESSURE DETECTED AND HUMIDITY > 90%, isRaining = true
 
 		bool isRaining = true;
 		Serial.println("RAIN DETECTED");
 		delay(10000);
 		
		}
	}
}