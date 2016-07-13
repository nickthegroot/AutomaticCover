/*
THE AUTOMATIC COVER
Version 2.0

Changelog for Version 2.0:
* Added more functions for better understanding
* Added better comments for better understanding
* Fixed bug where the override button would open the cover after the cover was opened by rain.
* Fixed bug where the cover would come in when not raining after override button was pushed
* Other minor changes

IMPORTANT:
Ports and needed libaries are listed in README.me

TO-DO:
1. Calabrate stepper motor to turn required amount.
*/

// INPUT

// INITIALIZE HUMIDITY SENSOR
#include "DHT.h"
#define DHTPIN 8     // Humidity digital pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.

// INITIALIZE LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

// INITIALIZE VARIABLES
bool coverOut;
int pressure;
float humidity;
bool overrideOut;
bool overrideButton;
unsigned long previousMillis = 0;
const long updateInterval = 10 * 60000; // Rain update frequency in minutes [minutes] * 60000
int dirpin = 6; // Pin for the "dir" slot on the stepper motor shield
int steppin = 7; // Pin for the "step" slot on the stepper motor shield
const int overridePin = 9;  // Pin for the override button
int stepsPerOpen = 10000; // The number of steps per the cover being fully extended
int pressureThreshold = 50; // The threshold for the pressure to activate
int humidityThreshold = 90: // The threshold for the humidity to activate (%)


// CUSTOM FUNCTIONS


// Controls the override button
void overrideCheck() {
  // Checks if the override button is pushed
  overrideButton = digitalRead(overridePin);
  coverOut = checkCover(coverOut);
  // If button is pushed and the cover is in, push the cover out
    if (overrideButton == LOW) {
      if (coverOut == false) {
        openCover();
        overrideActivated(true);
  }
  // If the button is pushed and the cover is out, pull the cover in
      else {
        closeCover();
        overrideActivated(false);
  }
 }
}

// Checks for pressure coming from rain drops
void pressureCheck() {

  int pressure = analogRead(A0);
  delay(50);

  // IF PRESSURE DETECTED, CHECK HUMIDITY
  if (pressure > pressureThreshold) {
   Serial.print("Pressure: ");
   Serial.println(pressure);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Pressure");
   lcd.setCursor(0,1);
   lcd.print("detected");
   delay(1000);
   humidityCheck();
   }
}

// Makes sure the humidity is above 90% (raining) when pressure is detected
void humidityCheck() {
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

  if (humidity > humidityThreshold) {

  // IF PRESSURE DETECTED AND HUMIDITY > 90%, coverOut = true

    Serial.println("Rain Detected");
    Serial.println("");
    lcd.clear();
    lcd.print("Rain");
    lcd.setCursor(0,1);
    lcd.print("detected");
    delay(2000);
    openCover();

  }
  else {

    // IF PRESSURE DETECTED AND HUMIDITY < 90%, coverOut = False

    Serial.println("False positive - humidity override");
    Serial.println("");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Humidity");
    lcd.setCursor(0,1);
    lcd.print("Override");
    delay(2000);
    }
}

// Makes sure rain is still falling by checking the humidity every __ minutes (can be controlled by changing)
void rainUpdate() {

  // Loop checking rain every __ minutes (default 10 minutes)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    bool overrideState = overrideActivated(overrideOut);

    // Only checks the humidity if the cover is out and the override switch has not been pressed
    if (overrideState = false) {
      if (coverOut = true) {
          humidityCheck();
          if (coverOut = false) {
            closeCover();
    }
   }
  }
 }
}

// Opens the cover
void openCover() {
  // Prints "opening cover" on the LCD
  checkCover(true);
  Serial.print("Opening Cover...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Opening");
  lcd.setCursor(0,1);
  lcd.print("Cover");
  // Activates the motor to push the cover out
  int i;
  digitalWrite(dirpin, LOW);     // Set the direction.
  delay(100);
  for (i = 0; i<stepsPerOpen; i++)
  {
  digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
  digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
  delayMicroseconds(500);      // This delay time is close to top speed for this
  }
  }                             // particular motor. Any faster the motor stalls.

// Closes the cover
void closeCover() {
  // Prints "Closing Cover" on the LCD
  coverOut = checkCover(false);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Closing");
  lcd.setCursor(0,1);
  lcd.print("Cover");
  // Activates the motor to pull the cover in
  int i;
  digitalWrite(dirpin, HIGH);    // Change direction.
  delay(100);
  for (i = 0; i<stepsPerOpen; i++)
  {
  digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
  digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
  delayMicroseconds(500);      // This delay time is close to top speed for this
  }                              // particular motor. Any faster the motor stalls.
  }

// Checks if the cover is pulled out or in
bool checkCover(bool coverOut) {
  return coverOut;
}

// Checks if the cover is pulled out from the override button
bool overrideActivated(bool overrideOut) {
  return overrideOut;
}


// BASE FUNCTIONS


void setup() {
  Serial.begin(9600);
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
  digitalWrite(dirpin, LOW);
  digitalWrite(steppin, LOW);
  pinMode(overridePin, INPUT);
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

overrideCheck();
pressureCheck();
rainUpdate();

}
