/*
 * ----------------------------------------------------------------------------
 *
 * Project: Autonomous Water Cleaning Robot
 * File:    main.ino (Main Program File)
 * Author:  La Min Maung
 * Date:    October 2025
 * License: MIT License
 * ----------------------------------------------------------------------------
 * Description:
 * Main control program for autonomous microcontroller-based robot that
 * collects floating trash from water surfaces.
 * The system operates based on a preset time entered by the user,
 * using RTC DS3231 for time tracking.
 *
 * Controller: ESP8266 NodeMCU
 * Libraries Used: RTClib, Servo, Keypad_I2C, LiquidCrystal_I2C, Wire
 *
 * ----------------------------------------------------------------------------
 */

#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <Keypad_I2C.h>
#include <LiquidCrystal_I2C.h>

// RTC Module
RTC_DS3231 rtc;

// LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);


// Default PCF8574 address
#define I2C_ADDR 0x20  
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3};  // PCF8574 Pins
byte colPins[COLS] = {4, 5, 6, 7};

Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2C_ADDR);


//Input Variables
String inputTime = "";
bool inputComplete = false;
int HH = -1, MM = -1, S = -1;


// Motor and Conveyor System
#define RELAYConveyor 1  // TX

// L298N Motor Driver
#define MOTOR_IN1 D5
#define MOTOR_IN2 D6
#define MOTOR_IN3 D7
#define MOTOR_IN4 D8


// Ultrasonic Sensor
#define TRIG_PIN D3
#define ECHO_PIN 3 //RX
Servo sensorServo;

// Alarm System
#define BUZZER D0


// Operation variables
int leftDistance, rightDistance,middleDistance;
bool isOperating = false;

void setup() {
  
    Wire.begin(D2, D1);  // ESP8266 I2C (SDA, SCL)
    rtc.begin();
    //lcd.init();
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Robot Boat Ready");
    
   
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RELAYConveyor, OUTPUT);
    sensorServo.attach(D4);
    
  
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
  
    keypad.begin();
}

void loop()
{
 digitalWrite(BUZZER, HIGH);
    if (!inputComplete) 
     { 
        getInputTime();
     } 
    else 
     {  char key = keypad.getKey();
        if (key) {
            if (key == '*') {
                lcd.setCursor(0,0);
                lcd.print("                ");
                lcd.setCursor(0,1);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Restarting...");
                delay(1000); // Short delay to show message
                ESP.restart(); // Restart the ESP8266
            }
        }


      
        DateTime now = rtc.now();
        printTime(now.hour(), now.minute(), now.second());
            lcd.print("    ");
            lcd.setCursor(0,0);
            lcd.print("RTC:");
          
            
        if (!isOperating && now.hour() == HH && now.minute() == MM && now.second() == S) 
        {   
            isOperating = true;
             startOperation();
             displayEnd();
             char key = keypad.getKey();
             if (key) {
              if (key == '*') {
                lcd.setCursor(0,0);
                lcd.print("                ");
                lcd.setCursor(0,1);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Restarting...");
                delay(1000); // Short delay to show message
                ESP.restart(); // Restart the ESP8266
                }
               }
            
        }
        if (isOperating && now.hour() == HH+2 && now.minute() == MM && now.second() == S)
               {
                stopOperation();
                isOperating = false;
               
               }
    
        if (isOperating) 
        {
            scanForPath();
        }
     }
    
}

void getInputTime() 
{ 
    lcd.setCursor(0, 1);
    lcd.print("HHMMSS: ");
    inputTime = "";

    while (inputTime.length() < 6 ) 
    {
        yield();
        char key = keypad.getKey();
        if (key) 
        {
            if (key >= '0' && key <= '9') 
            {
                inputTime += key;
                lcd.setCursor(8 + inputTime.length(), 1);
                lcd.print(key);
            } 
            else if (key == '#') 
            {
                inputTime = "";
                lcd.setCursor(8, 1);
                lcd.print("      ");
            }
        }
    }

    HH = (inputTime[0] - '0') * 10 + (inputTime[1] - '0');
    MM = (inputTime[2] - '0') * 10 + (inputTime[3] - '0');
    S = (inputTime[4] - '0') * 10 + (inputTime[5] - '0');

    if (HH < 24 && MM < 60 && S < 60) 
    {
        lcd.setCursor(0, 1);
        lcd.print("SetTime:");
        printTime(HH, MM, S);
        inputComplete = true;
    } 
    else 
    {
        lcd.setCursor(0, 1);
        lcd.print("Invalid! Try Again");
        delay(2000);
        lcd.setCursor(0,1);
        lcd.print("                      ");
        HH = MM = S = -1;
        inputComplete = false;
    }
}


void printTime(int hour, int minute, int second) 
{
    if (hour < 10) lcd.print("0");
    lcd.print(hour);
    lcd.print(":");
    if (minute < 10) lcd.print("0");
    lcd.print(minute);
    lcd.print(":");
    if (second < 10) lcd.print("0");
    lcd.print(second);
     lcd.print("          ");
}

void displayEnd()
{
     lcd.setCursor(0,1);
     lcd.print("             ");
     lcd.setCursor(0,1);
     lcd.print("End:");
     printTime(HH+2,MM,S);
     lcd.print("             ");
}    
void startOperation() 
{
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.print("Operation Start");
    digitalWrite(RELAYConveyor, HIGH);
    moveForward();
}

void stopOperation() 
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Operation Stop");
    digitalWrite(RELAYConveyor, LOW);
    stopMotors();
    triggerAlarm();
    isOperating = false;
    inputComplete = false;
}
int measureDistance() 
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    return (duration > 0) ? duration * 0.034 / 2 : 100;
}

void scanForPath() {
    sensorServo.write(0);
    delay(500);
    leftDistance = measureDistance();
    
    sensorServo.write(180);
    delay(500);
    rightDistance = measureDistance();
    
    sensorServo.write(90);
    delay(500);
    middleDistance= measureDistance();
    
     if (leftDistance > rightDistance && leftDistance < 40) {
        turnLeft();
    } else if (rightDistance > leftDistance && rightDistance < 40) {
        turnRight();
    } else if(middleDistance < 40){
           moveBackward();
              if(leftDistance> rightDistance)
              { turnLeft(); }
              if(leftDistance< rightDistance)
              { turnRight(); }
        
    }
    if (leftDistance>40 && rightDistance>40 & middleDistance>40)
    { sensorServo.write(90);}
}

void moveForward() {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
 
    lcd.setCursor(15,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print("F");
}

void moveBackward() {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
   
    lcd.setCursor(15,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print("B");
}

void turnRight() {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
    lcd.setCursor(15,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print("R");
}

void turnLeft() {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
     lcd.setCursor(15,1);
    lcd.print(" ");
    lcd.setCursor(15,1);
    lcd.print("L");
}

void stopMotors() {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
}

void triggerAlarm() {
    digitalWrite(BUZZER, LOW);
    delay(10000);
    digitalWrite(BUZZER, HIGH);
   
}
