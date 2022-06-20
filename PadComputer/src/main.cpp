#include <Arduino.h>
#include <Servo.h>


 int servoPin = A0;
  Servo launchClamp;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  launchClamp.attach(servoPin);
  launchClamp.write(0);
  delay(15000);
  launchClamp.write(180);
}



void loop() {
  // put your main code here, to run repeatedly:
  // String command = Serial.readString();
  // if(command == "Send It!"){
  //   delay(200);
  //   launchClamp.write(180);
  //   delay(500);
  //}
}