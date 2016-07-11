/*
THE AUTOMATIC COVER
Version 1.6

ChangeLog:
1.0 - Compleated all sensor + LCD work.
1.05 - Started work on stepper motor output
1.1 - More stepper motor work
1.2 - ???
1.3 - Updated LCD to show cover opening / closing
1.4 - Bug fixes, timing changes, and checking for rain only when not raining.
1.41 - Minor formatting changes
1.42 - Minor fixes
1.5 - Added override button and other minor changes
1.51 - Minor changes
1.6 - Changed the way the override button works

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
int overrideState;
int coverOpening;
unsigned long previousMillis = 0;
const long interval = 10 * 60000; // Rain update frequency in minutes [minutes] * 60000
int dirpin = 6; // Pin for the "dir" slot on the stepper motor shield
int steppin = 7; // Pin for the "step" slot on the stepper motor shield
const int overridePin = 9;  // Pin for the override button
int stepsPerOpen; // The number of steps per the cover being fully extended

// OPENS THE COVER

void openCover() {
  Serial.print("Opening Cover...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Opening");
  lcd.setCursor(0,1);
  lcd.print("Cover");
  int i;
digitalWrite(dirpin, LOW);     // Set the direction.
delay(100);
for (i = 0; i<4000; i++)       // Iterate for 4000 microsteps.
  {
  digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
  digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
  delayMicroseconds(500);      // This delay time is close to top speed for this
 }
}                             // particular motor. Any faster the motor stalls.

void closeCover() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Closing");
  lcd.setCursor(0,1);
  lcd.print("Cover");
  int i;
digitalWrite(dirpin, HIGH);    // Change direction.
delay(100);
for (i = 0; i<4000; i++)       // Iterate for 4000 microsteps
{
  digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
  digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
  delayMicroseconds(500);      // This delay time is close to top speed for this
}                              // particular motor. Any faster the motor stalls.
}

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

  // IF PRESSURE DETECTED AND HUMIDITY > 90%, coverOut = true

    bool coverOut = true;
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

    bool coverOut = false;
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

// OVERRIDE BUTTON

overrideState = digitalRead(overridePin);

while (overrideState == LOW) {

// KEEPS CHECKING IF OVERRIDE BUTTON IS PRESSED

  overrideState = digitalRead(overridePin);

// OPENS COVER UNTILL FULLY EXTENDED

  while (coverOpening > stepsPerOpen) {
    coverOpening++;
    digitalWrite(dirpin, LOW);     // Set the direction.
    digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(500);      // This delay time is close to top speed for this
}

// WAITS FIVE SECONDS IF COVER IS FULLY EXTENDED

  if (coverOpening == stepsPerOpen) {
    coverOut = true;
    delay(5000);
    coverOpening++;
  }

// CLOSES COVER UNTILL FULLY CLOSED

  while (coverOpening < stepsPerOpen * 2) {
    coverOpening++;
    digitalWrite(dirpin, HIGH);    // Set the direction
    digitalWrite(steppin, LOW);  // This LOW to HIGH change is what creates the
    digitalWrite(steppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
    delayMicroseconds(500);      // This delay time is close to top speed for this
 }


 // WAITS FIVE SECONDS IF COVER IS FULLY CLOSED

 if (coverOpening == stepsPerOpen * 2) {
   coverOut = false;
   delay(5000);
   coverOpening = 0;
 }
}

// CHECK PRESSURE

 int pressure = analogRead(A0);
 delay(50);

 // IF PRESSURE DETECTED, CHECK HUMIDITY
 if (pressure > 20) {
  Serial.print("Pressure: ");
  Serial.println(pressure);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pressure");
  lcd.setCursor(0,1);
  lcd.print("detected");
  delay(1000);
  checkHumidity();
  }
// RAIN UPDATING + CLOSING COVER IF HUMIDITY < 90% EVERY 10 MINUTES

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  if (coverOut = true) {
    checkHumidity();
    if (coverOut = false) {
      closeCover();
      }
    }
  }
}
