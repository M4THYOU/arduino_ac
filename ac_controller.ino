#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

const int sensorPin = A0;
int currentTemp = 0;
int intendedTemp;

const int fanPin = 8;
bool isFanOn = false;

const int incButtonPin = 9;
const int decButtonPin = 10;

void setup() {
  Serial.begin(9600);

  pinMode(fanPin, OUTPUT);
  pinMode(incButtonPin, INPUT);
  pinMode(decButtonPin, INPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("hello, world!");

  delay(3000);

  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0) * 5.0;
  float temp = (voltage - .5) * 100;
  intendedTemp = 26; // set to 26 initially
}

void loop() {

  int incButtonState = digitalRead(incButtonPin);
  int decButtonState = digitalRead(decButtonPin);

  bool intendedTempChanged = false;
  if (incButtonState == HIGH && decButtonState == LOW) {
    intendedTempChanged = true;
    intendedTemp += 1;  
  }else if (decButtonState == HIGH && incButtonState == LOW) {
    intendedTempChanged = true;
    intendedTemp -= 1;
  }

  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal/1024.0) * 5.0;
  float temp = (voltage - .5) * 100;
  
  int roundedTemp = round(temp);

  if (roundedTemp != currentTemp || intendedTempChanged) {
    currentTemp = roundedTemp;

    String realTempText;
    realTempText += String(roundedTemp); 
    realTempText += String((char)223);
    realTempText += "C";

    String intendedTempText;
    intendedTempText += String(intendedTemp);
    intendedTempText += String((char)223);
    intendedTempText += "C";
  
    lcd.clear();
    lcd.print(intendedTempText);
    lcd.setCursor(12, 0);
    lcd.print(realTempText);
  }

  if (intendedTemp >= currentTemp && isFanOn) {
    // turn fan off
    digitalWrite(fanPin, LOW);
    isFanOn = false;
  } else if (intendedTemp < currentTemp && !isFanOn) {
    // turn fan on
    digitalWrite(fanPin, HIGH);
    isFanOn = true;
  }

  delay(300);

}
