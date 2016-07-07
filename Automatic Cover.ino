/* 
THE AUTOMATIC COVER - SENSOR EDITION

Ports:
Pressure - Analog pin A0
Humidity - Digital pin 2
*/

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

const long interval = 3000;
unsigned long previousMillis = 0;  

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {

// HUMIDITY

  unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity!");
    return;
  }
}
 // PRESSURE

 int pressure = analogRead(A0);
 delay(500);
 
 // SERIAL PRINT READINGS
 
 Serial.print("Pressure: ");
 Serial.print(pressure);
 Serial.print("		");
 Serial.print("Humidity: ");
 Serial.println(humidity);
}