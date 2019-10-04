int compassValue;
int compassSensor;
int heading;

int PSDSensor;
int PSDValue;


void setup() {
  
  heading = analogRead(compassSensor);
  
  compassSensor = A0;
  compassValue = 0;

  PSDSensor = 
}

void loop() {

  compassValue = analogRead(compassSensor);
  if(compassValue > heading){
    //adjust motors to head left/right
  }
  if(compassValue < heading){
    //adjust motors to head right/left
  }
  else{
    //turn both motors on
  }

  delay(100);
}
