/* Overland Flow Sensor Software 
 * Version 0.1
 * 
 * This software collects range data from two IR sensors and provides
 * latitude, longitude, and datetime information. The data is saved on 
 * a micro-SD card with datetime formated for SQL.
 * 
 * The minimum range of the IR sensor is 4 cm and maximum range is ~20cm.
 * Analog range values are converted to distance by the lookup table values
 * with the key:value pair is array index is the analog voltage, and the 
 * array value is the calibrated distance. The lookup tables are stored in 
 * the program memory of the Arduino.
 */

//IR sensor analog pins
#define irSensorPin0   A0  
#define irSensorPin1   A2 
int irSensor0=0;
int irSensor1=0;
bool flag = false;

// Calibration table motor control
int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin

//button switch 
const int buttonPin=4;     // the number of the pushbutton pin
bool buttonState=0;         // variable for reading the pushbutton status\\


//Sensor lookup Table
int Sensor0[50] = {}; 
int Sensor1[50] = {};


void driveForward(){
     for (int i=0; i<800; i++)    //Forward 1 mm
     {
        digitalWrite(DIR,LOW);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(50);
        digitalWrite(PUL,LOW);
        delayMicroseconds(50);  
     }
}

void driveReverse(){
   for (int i=0; i<800; i++)    //Reverse 1 mm
     {
        digitalWrite(DIR,HIGH);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(50);
        digitalWrite(PUL,LOW);
        delayMicroseconds(50);  
     }
}

void driveStop(){
   for (int i=0; i<800; i++)    //Reverse 1 mm
     {
        digitalWrite(DIR,HIGH);
        digitalWrite(ENA,LOW);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(50);
        digitalWrite(PUL,LOW);
        delayMicroseconds(50);  
     }
}

void setStage(){
  while(buttonState==LOW){
     driveReverse();
     //driveForward();
  buttonState = digitalRead(buttonPin); 
  }
}


void setup() {
  Serial.begin(115200);  
  // initialize stepper motor  
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);   
  setStage(); //intitialize stage
}

int count = 0;

void loop() {  
  
  while( count < 50){
    driveForward();
    irSensor0 = analogRead(irSensorPin0);
    irSensor1 = analogRead(irSensorPin1);

    Sensor0[count] = irSensor0;
    Sensor1[count] = irSensor1;
    delay(750);
    count++;
  }
 if((count==50)&&(flag == false)){
  for(int i=0; i<50; i++){
    Serial.print(Sensor0[i]);
    Serial.print(",");
    Serial.println(Sensor1[i]); 
  }
  flag = true;
 }
}
