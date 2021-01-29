/*
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * Button pin to analog pin A0
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

Servo motorA, motorB;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int motorAValue = 0;
int motorBValue = 0;

void setup()
{

  Serial.begin(115200);
  Serial.print("Starting...\n");

  lcd.begin(16, 2);
  lcd.print("Motor Tester");
  lcd.setCursor(0, 1);
  lcd.print("A: ");
  lcd.print(motorAValue);
  lcd.setCursor(8, 1);
  lcd.print("B: ");
  lcd.print(motorBValue);

  motorA.attach(A1);
  motorB.attach(A2);

  motorA.write(92);
  motorB.write(92);
}

unsigned static long debounceInterval = 80;
unsigned long debounceMillis = 0;
uint8_t lastButtonState = 0;

uint8_t checkButton()
{
  uint8_t button = 0;
  uint8_t returnButton = 0;
  analogReference(DEFAULT);
  long val = analogRead(A0);
  if (val < 1000)
  {
    if (val >= 0)
      button = 3; // right
    if (val >= 130)
      button = 4; // up
    if (val >= 310)
      button = 5; // down
    if (val >= 490)
      button = 6; // left
    if (val >= 720)
      button = 7; // select
  }
  if (val >= 1000)
    button = 0; // not pressed

  unsigned long currentMillis = millis();
  if (currentMillis - debounceMillis > debounceInterval)
  {
    if (button > 0)
    {
      if (button == lastButtonState)
      {
        returnButton = button;
      }
    }
    lastButtonState = button;
    debounceMillis = currentMillis;
  }
  return returnButton;
}

uint8_t activeSide = 1;

void clearMotorValues()
{
  lcd.setCursor(10, 1);
  lcd.print("    ");
  lcd.setCursor(2, 1);
  lcd.print("    ");
}

void writeMotorValues()
{
  clearMotorValues();
  if (motorAValue < 0)
  {
    lcd.setCursor(2, 1);
  }
  else
  {
    lcd.setCursor(3, 1);
  }
  lcd.print(motorAValue);
  if (motorBValue < 0)
  {
    lcd.setCursor(10, 1);
  }
  else
  {
    lcd.setCursor(11, 1);
  }
  lcd.print(motorBValue);
  // reset the blink position...
  if (activeSide == 1)
    lcd.setCursor(0, 1);
  if (activeSide == 2)
    lcd.setCursor(8, 1);

  // motorA.write(map(motorAValue, -100, 100, 0, 179));
  motorA.write(motorAValue + 92);
  motorB.write(motorBValue + 92);
}

void doUp()
{
  if (activeSide == 1)
  {
    motorAValue = motorAValue > 90 ? 90 : motorAValue + 1;
  }
  if (activeSide == 2)
  {
    motorBValue = motorBValue > 90 ? 90 : motorBValue + 1;
  }
  writeMotorValues();
  Serial.println("up");
}

void doLeft()
{
  Serial.println("left");
  lcd.setCursor(0, 1);
  lcd.blink();
  activeSide = 1;
}

void doDown()
{
  if (activeSide == 1)
  {
    motorAValue = motorAValue < -90 ? -90 : motorAValue - 1;
  }
  if (activeSide == 2)
  {
    motorBValue = motorBValue < -90 ? -90 : motorBValue - 1;
  }
  writeMotorValues();
  Serial.println("down");
}

void doRight()
{
  Serial.println("right");
  lcd.setCursor(8, 1);
  lcd.blink();
  activeSide = 2;
}

void doSelect()
{
  Serial.println("select");
}

unsigned static long secondInterval = 1000;
unsigned long secondMillis = 0;

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - secondMillis > secondInterval)
  {
    secondMillis = currentMillis;
  }
  uint8_t buttonPressed = checkButton();
  switch (buttonPressed)
  {
  case 0:
    break;
  case 3:
    doRight();
    break;
  case 4:
    doUp();
    break;
  case 5:
    doDown();
    break;
  case 6:
    doLeft();
    break;
  case 7:
    doSelect();
    break;
  }
}