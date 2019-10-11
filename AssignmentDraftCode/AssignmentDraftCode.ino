#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define COMPASSPIN_SCL 7 //THESE NEED TO CHANGE TO SCL AND SDA PINS
#define COMPASSPIN_SDA 6 //DONT need to specify pins for the compass readings
#define PSDPIN 14

#define TURN ~~     //Determine to indicate when boat should begin turning

//Assigning unique ID to sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float compassValue;
float heading;

byte PSDValue;

void turning(){
  
  mag.getEvent(&event);
  float compassValue = atan2(event.magnetic.y, event.magnetic.x);
  
  while(compassValue != heading){
    
    // left motor on, right motor reverse

    mag.getEvent(&event);
    float compassValue = atan2(event.magnetic.y, event.magnetic.x);
  }
  // reset the in pins for h-bridge
}

void setup() {
  
  //Initialise the sensor
  mag.begin();
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
   
}

void loop() {

  
  mag.getEvent(&event);
  float compassValue = atan2(event.magnetic.y, event.magnetic.x);

  if(PSDValue > TURN){
    heading += PI;
    if(heading > 2 * PI){
      heading -= 2 * PI;
    }
  }
  
  
  if(compassValue > heading){
    //Turn right motor on, left motor off
  }
  if(compassValue < heading){
    //Turn left motor on, right motor off
  }
  else{
    //turn both motors on
  }

  delay(100);
}
