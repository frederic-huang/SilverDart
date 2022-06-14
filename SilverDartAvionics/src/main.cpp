#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <PWMServo.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

//Defining Pin Numbers for Launch Clamp Servos
const int clampServoPin = A0;

//Defining Pin Numbers for Parachute Deployment Mechanism Servos
const int paraMechPin = A9;

//Defining Pins for BMP280 
#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)


//Defining Servos
PWMServo clampServos;
PWMServo chuteMech;

//Temporary storage for reading from EEPROM
double data = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Flight Computer in Startup");

  //Output all stored flight data fro last flight
  if(EEPROM.read(0) != 255 || EEPROM.read(0) != 0){
    Serial.println("**Start of Last Flight Data**");
    for(int i=0; i<2048; i++){
      EEPROM.get(i, data);
      Serial.println(data);
    }
    Serial.println("**End of Last Flight Data**");
  } else{
    Serial.println("*No previous flight data*");
  }

  //Erase EEPROM Data
  for(int i=0; i<2048; i++){
    EEPROM.write(i, 0);
  }
  Serial.println("Previous Flight Data Cleared");

  //Servo Setup
  clampServos.attach(clampServoPin);
  chuteMech.attach(paraMechPin);

  //Altimeter Setup

  delay(300000);
}

int startTime = millis();
int delayTime = 1000;

void loop() {
  // put your main code here, to run repeatedly:
  if((millis()-startTime)==delayTime){
    startTime = millis();
  }
}