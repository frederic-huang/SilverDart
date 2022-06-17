#include <Arduino.h>
#include <PWMServo.h>

const int paraMechPin = A9;
PWMServo chuteMech;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10000);
  chuteMech.attach(paraMechPin);
  chuteMech.write(0);
  delay(5000);
  chuteMech.write(180);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
}