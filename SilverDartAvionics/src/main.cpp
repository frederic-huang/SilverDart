#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <PWMServo.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>


//Defining Pin Numbers for Parachute Deployment Mechanism Servos
const int paraMechPin = A9;

//Defining Servos
PWMServo clampServos;
PWMServo chuteMech;

//Altimeter Setup
#define BMP_SCK  (15)
#define BMP_MISO (16)
#define BMP_MOSI (17)
#define BMP_CS   (18)

//Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

  

//Temporary storage for reading from EEPROM
// double data = 0;

  float startAltitude;
  float temperature;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // EEPROM.begin();
  delay(10000);
  Serial.println("Flight Computer in Startup");

  //Output all stored flight data fro last flight
  // if(EEPROM.read(0) != 255 || EEPROM.read(0) != 0){
  //   Serial.println("**Start of Last Flight Data**");
  //   for(int i=0; i<2048; i++){
  //     EEPROM.get(i, data);
  //     Serial.println(data);
  //   }
  //   Serial.println("**End of Last Flight Data**");
  // } else{
  //   Serial.println("*No previous flight data*");
  // }

  //Erase EEPROM Data
  // for(int i=0; i<2048; i++){
  //   EEPROM.write(i, 0);
  // }
  Serial.println("Previous Flight Data Cleared");

  //Servo Setup
  chuteMech.attach(paraMechPin);
  
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1000); /* Standby time. */
  delay(180000);
  if(bmp.takeForcedMeasurement()){
    startAltitude = bmp.readPressure();
    temperature = bmp.readTemperature();
    Serial.println(bmp.readTemperature());
    Serial.println(bmp.readPressure());
  }
  Serial.println(startAltitude);
  Serial.println(temperature);
  // EEPROM.put(0, startAltitude);
  // EEPROM.put(1, temperature);
  delay(10000);
}

unsigned long startTime = millis();
unsigned long delayTime = 1000;
float maxAltitude;
float previousAltitude;
float currentAltitude;

//States
boolean ascent = false;
boolean descent = false;

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  //if((millis()-startTime) == delayTime){        //Ensure time interval is met
    startTime = millis();                       //Sets new start time for next time interval check
    previousAltitude = currentAltitude;         //Stores the most recent altitude as the previous altitude
    if(bmp.takeForcedMeasurement()){
      currentAltitude = bmp.readPressure();         //New altitude pressure is read
      Serial.println(currentAltitude);
    }         
    if(currentAltitude < previousAltitude){       //Comparison made for ascent state check
      delay(250);
      if(bmp.takeForcedMeasurement()){
        currentAltitude = bmp.readPressure();
        Serial.println(currentAltitude);
      }
      if(currentAltitude < previousAltitude){     //Double check to verify
        ascent = true;                            //Sets state to ascent
      }
    }
    if(ascent == true){
      if(currentAltitude > previousAltitude){     //Comparison made to check for descent
        maxAltitude = previousAltitude;           //Sets previous altitude as max altitude
        delay(500);
        if(bmp.takeForcedMeasurement()){
          currentAltitude = bmp.readPressure();
          Serial.println(currentAltitude);
        }
        if(currentAltitude > previousAltitude){   //Double check to verify vehicle is descending
          descent = true;                         //Sets state to descent
          chuteMech.write(180);                   //Deploy parachute
          while(true){
            Serial.println(maxAltitude);
            Serial.println(ascent);
            Serial.println(descent);
            delay(2500);
          }
        }
      }
    }
    //Store max altitude data to EEPROM
    // if(descent == true){
    //   delay(15000);
    //       EEPROM.put(2, maxAltitude);
    //   delay(120000);
    // }
    
 // }

}