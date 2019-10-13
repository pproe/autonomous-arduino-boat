#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// ## THIS SECTION CONTAINS ADJUSTABLE COMPONENTS ##
float turn = 10;     //Indicate when boat should begin turning
int delayAmount = 500; //Indicate how long between each update of direction

//Assigning unique ID to sensor
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float compassValue;
float heading;
bool turned = false;
byte PSDValue;
unsigned long timeOfTurn;

//Left Motor Connections
int enA = 6;
int in1 = 7;
int in2 = 4;

//Right Motor Connections
int enB = 5;
int in3 = 2;
int in4 = 3;

//Initialise the sensor
sensors_event_t event;

void setup() {

  // set both motors to maximum speed {PWM 0-255}
  analogWrite(enA, 255); 
  analogWrite(enB, 255);
  
  //Setup of all motor control pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //Inital State of motors - All off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  
  mag.begin();
  mag.getEvent(&event);
  
  //record initial heading value
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
}

void loop() {
  
  //attain new reading from compass
  mag.getEvent(&event);
  float compassValue = atan2(event.magnetic.y, event.magnetic.x);

  //Attain PSD value
  PSDValue = analogRead(A0);
  
  if(PSDValue > turn && !turned){
    //changes heading value to be opposite direction and initiates turning procedure
    heading += PI;
    if(heading > 2 * PI){
      heading -= 2 * PI;
    }
    turning();
  }

   if(PSDValue > turn && turned && millis() - timeOfTurn > 3000){  
    //turns off motor if the boat reaches the second wall it sees
    //also checks if it has been at least 3000ms since turn
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  
  if(compassValue > heading){
    //Turn right motor on, left motor off
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  
  if(compassValue < heading){
    //Turn left motor on, right motor off
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);   
  }
  
  else{ 
    // turn both motors on
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  
  delay(delayAmount);
}

void turning(){
  
  turned = true;

  //attain new compass reading
  mag.getEvent(&event);
  float compassValue = atan2(event.magnetic.y, event.magnetic.x);
  
  while(compassValue != heading){
    // left motor on, right motor reverse
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //attain new compass reading
    mag.getEvent(&event);
    float compassValue = atan2(event.magnetic.y, event.magnetic.x);
  }
  
  //turn motors off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  //record time of turn to prevent false positive of second wall detection
  timeOfTurn = millis();
  
}
