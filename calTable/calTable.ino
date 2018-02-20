
/* Overland Flow Sensor Calibration Table
 * 
 * This software collects range data from two IR sensors and provides
 * ranging data on a linear stage
 * 
 * The minimum range of the IR sensor is 4 cm and maximum range is ~20cm.
 * For use with a linear stage and stepper motor control
 */
#include<math.h>

//IR sensor analog pins
#define irSensorPin0   A0  
#define irSensorPin1   A2 
#define ranging 100 //mm

int irSensor0=0;
int irSensor1=0;
bool flag = false;
double sensor1 = 0.0;
double sensor2 = 0.0;

// Calibration table motor control
int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin

//button switch on calibration table
const int buttonPin=4;     // the number of the pushbutton pin
bool buttonState=0;         // variable for reading the pushbutton status\\

//Sensor array table
int Sensor0[ranging] = {}; 
int Sensor1[ranging] = {};


//linear stage functions
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

//Set stage to button position
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

//loop counters
int count= 0;
int countR= 0;

void loop() {  
  while( count < ranging){
    driveForward();
    irSensor0 = analogRead(irSensorPin0);
    irSensor1 = analogRead(irSensorPin1);
    Sensor0[count] = irSensor0;
    Sensor1[count] = irSensor1;
   //range test 
   /*sensor1=30772*pow(irSensor0,-1.039)+4;
   sensor2=23522*pow(irSensor1,-0.973);
   Serial.print((sensor1));
   Serial.print(",");
   Serial.print((sensor2));
   Serial.print(",");
   Serial.println(((sensor1+sensor2)/2));*/
    delay(750);
    count++;
  }

/*print sensor arrays data captured at 1mm */ 
 if((count==ranging)&&(flag == false)){
  for(int i=0; i<ranging; i++){
    Serial.println("Done!");
    Serial.print(Sensor0[i]);
    Serial.print(",");
    Serial.println(Sensor1[i]); 
  }
  flag = true;
 }
}
