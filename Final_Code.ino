#include <LSM303.h>
#include <Wire.h>


// ## THIS SECTION CONTAINS ADJUSTABLE COMPONENTS ##
float turn = 40;     //Indicate when boat should begin turning (cm)
int delayAmount = 250; //Indicate how long between each update of direction


int currentHeading;

//declaration of Ultrasonic Sensor values
const int trig = 8;
const int echo = 9;


bool turned = false;
unsigned long timeOfTurn;
unsigned long timeOfStart;

//Left Motor Connections
int enA = 6;
int in1 = 7;
int in2 = 4;

//Right Motor Connections
int enB = 5;
int in3 = 2;
int in4 = 3;


// create compass object
LSM303 compass;

void setup() {

  //for testing/debugging
  Serial.begin(9600);

  // set both motors to half speed {PWM 0-255}
  analogWrite(enA, 255); 
  analogWrite(enB, 255);
  
  //Setup of all motor control pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //Setup of Ultrasonic sensor inputs
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);

  //Inital State of motors - All off
  Serial.println("Both Motors off");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  

  //initialize compass
  Wire.begin();
  compass.init();
  compass.enableDefault();  

  //calibrate compass and record initial heading value

  

  compass.m_min = (LSM303::vector<int16_t>){  -332,   -707,   -929};
  compass.m_max = (LSM303::vector<int16_t>){  +978,   +461,   +786};
  
  compass.read();
  currentHeading = round(compass.heading());


  Serial.print("heading = ");
  Serial.println(currentHeading);

  timeOfStart = millis();
  
}

void loop() {

  long PSDValue;

  //attain new reading from compass
  compass.read();
  int compassValue = round(compass.heading());
/*
  if(abs(compassValue - currentHeading) > 180){
    if(compassValue < currentHeading){
      compassValue += 360;
    }
    else{
      compassValue -= 360;
    }
  }
*/
  Serial.print("compassValue = ");
  Serial.println(compassValue);

  //Attain PSD value
  PSDValue = distance();
  Serial.print("PSDValue = ");
  Serial.println(PSDValue);
  
  if(PSDValue < turn && !turned && millis() - timeOfStart > 3000){
    turned = true;
    
    //changes heading value to be opposite direction and initiates turning procedure
    currentHeading = 280; //+= 180;
    /*
    if(currentHeading > 360){
      currentHeading -= 360;
    }
    */
    Serial.print("new heading = ");
    Serial.println(currentHeading);
    turning();
  }

   if(PSDValue < 20 && turned && millis() - timeOfTurn > 5000){  
    //turns off motor if the boat reaches the second wall it sees
    //also checks if it has been at least 3000ms since turn
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    while(1){
      // infinite loop. boat stops moving
      delay(10000);
    }
  }


  
  if(compassValue < currentHeading){ 

    //Turn right motor on, left motor off
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW); 
    digitalWrite(in4, LOW); 
  }
  else if (compassValue > currentHeading){

    //Turn left motor on, right motor off
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

  }
  else{ 

    // turn both motors on
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  

  delay(delayAmount);
  
}

void turning(){
  Serial.println("Turning initiated");
  

  //attain new compass reading
  compass.read();
  int compassValue = round(compass.heading()); 
  while(compassValue < currentHeading-5 || compassValue > currentHeading + 5){ 
    
    //TODO adjust to account for error in measurement
    // left motor on, right motor reverse
    /*
    if(abs(compassValue - currentHeading) > 180){
      if(compassValue < currentHeading){
        compassValue += 360;
      }
      else{
        compassValue -= 360;
      }
    }
    */
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //attain new compass reading

    //compassValue++;

    compass.read();
    compassValue = round(compass.heading()); 

    Serial.print(compassValue);
    Serial.print(" : ");
    Serial.println(currentHeading);
    
  }
  
  //turn motors off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  
  //record time of turn to prevent false positive of second wall detection
  timeOfTurn = millis();
  
  //delay(delayAmount);
  
}
/*
int rounded(float val){ //helper function: rounds to nearest 2
  val = val/2;
  val = round(val);
  val = val*2;
  int int_val = (int) val;
  return int_val;
}
*/


long distance(){
  long dist, time;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);

  time = pulseIn(echo, HIGH); 
  dist = (time/2) / 29.1;

  if(dist < 10){
    dist = 100;
  }

  return dist;

}
