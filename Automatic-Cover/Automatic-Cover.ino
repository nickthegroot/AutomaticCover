/*
THE AUTOMATIC COVER
Version 1.4

ChangeLog:
1.0 - Compleated all sensor + LCD work.
1.05 - Started work on stepper motor output
1.1 - More stepper motor work
1.3 - Updated LCD to show cover opening / closing
1.4 - Bug fixes, timing changes, and checking for rain only when not raining.

Ports:
Pressure - Analog pin A0
Humidity (DHT22) - Digital pin 8 (3.3v)
LCD - Digital pins 2, 3, 4, 5, 11, and 12

NEEDED LIBARIES:
DHT Sensor Libary: https://github.com/adafruit/DHT-sensor-library
*/

/*
NOTES:
dirpin is directional
steppin is how much you want to move
~800 steps = one full revolution

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

// INITIALIZE STEPPER
#include <Stepper.h>
const int stepsPerRevolution = 200;  // Number of steps per revolution
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); // Pins for stepper motor

// INITIALIZE VARIABLES
bool isRaining;
int pressure;
float humidity;
unsigned long previousMillis = 0;
const long interval = 60000; // Rain update frequency (ms)
int dirpin = 2;
int steppin = 3;

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

  // IF PRESSURE DETECTED AND HUMIDITY > 90%, isRaining = true

    bool isRaining = true;
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

    // IF PRESSURE DETECTED AND HUMIDITY < 90%, isRaining = False

    bool isRaining = false;
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

// ONLY CHECK FOR PRESSURE IF IT'S NOT RAINING

while (isRaining = false) {

// CHECK PRESSURE

 int pressure = analogRead(A0);
 delay(50);

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
}
// RAIN UPDATING + CLOSING COVER IF HUMIDITY < 90% EVERY 10 MINUTES

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  if (isRaining = true) {
    checkHumidity();
    if (isRaining = false) {
      closeCover();
    }
    }
  }
}
