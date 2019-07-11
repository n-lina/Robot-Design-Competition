
#include <Arduino.h>


//make KP and KD adjustable by potentiometer? 

#define L_TAB PA_0
#define L_SPLIT PA_1 //2
#define PHOTO_0 PA_3
#define PHOTO_1 PA_4
#define R_SPLIT PA_5
#define R_TAB PA_7
#define L_MOTOR_FORWARD PA_6
//#define L_MOTOR_BACKWARD PA_2
#define R_MOTOR_FORWARD PB_0
//#define R_MOTOR_BACKWARD PB_1
#define KP 31 // PID proportion constant // 205 is  too high 
#define KD 11 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define SPLIT_THRESHOLD 300
#define TAB_THRESHOLD 200
 
int derivative; 
int timeStep=0; 
int position; 
int lastPosition=0; 
int PID; 
int number;
bool leftSensor;
bool rightSensor;
bool leftSplit;
bool rightSplit;
bool leftTab; 
bool rightTab;
int loopCounter =0;
void turnLeft();
void turnRight();
void stop();

//int state = 0;
//investigate adding rotary encoder next  

void setup()
{
 Serial.begin(9600);
 Serial.println("hello");
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_SPLIT, INPUT_PULLUP);
 pinMode(R_SPLIT, INPUT_PULLUP);
 pinMode(L_TAB, INPUT_PULLUP);
 pinMode(R_TAB, INPUT_PULLUP);
 pinMode(L_MOTOR_FORWARD, OUTPUT);
 pinMode(R_MOTOR_FORWARD, OUTPUT);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
}

void loop()
{
 leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
 rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
 timeStep++;

 if (leftSensor && rightSensor){
   position = 0;
 } 
 else if(leftSensor && !rightSensor){
   position = 1; 
 }
 else if(!leftSensor && rightSensor){
   position = -1; 
 }
 else{
   if(lastPosition<0) { //right was on last 
   position = -5; 
   }
   else { // last Position > 0 ==> left was on last 
   position = 5;
   } 
  }

 derivative = (position - lastPosition) / timeStep; 
 PID = (KP * position) + (KD * derivative); 

 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+PID, 0);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-PID, 0); 

 if(lastPosition != position){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
 }
 lastPosition = position;

 loopCounter++;
 
 if(loopCounter%10 == 0){
   leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
   rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
   leftSplit = analogRead(L_SPLIT) >=SPLIT_THRESHOLD;
   rightSplit = analogRead(R_SPLIT) >=SPLIT_THRESHOLD;
   leftTab =  analogRead(L_TAB)>=TAB_THRESHOLD;
   rightTab = analogRead(R_TAB)>=TAB_THRESHOLD;

   if((leftSplit || rightSplit) && (leftSensor && rightSensor) && (!leftTab && !rightTab)){
     Serial.println("split");
     stop();
     delay(1000); 
    }
   else if((leftSplit && leftTab) && (leftSensor && rightSensor) && (!rightSplit && !rightTab)){
     Serial.println("left tab");
     stop();
     delay(1000); 
     turnRight();
     delay(3000);
   }
   else if((rightSplit && rightTab) && (leftSensor && rightSensor) && (!leftSplit && !leftTab)){
     Serial.println("right tab");
     stop();
     delay(1000); 
     turnLeft();
     delay(3000);
   }
   loopCounter =0;
  }
}

void turnLeft(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-(5*KP),0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+(5*KP), 0); 
}

void turnRight(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-(5*KP),0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+(5*KP), 0); 
}

void stop(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
}


/*
#define L_TAB PA_0
#define L_SPLIT PA_1 //2
#define PHOTO_0 PA_3
#define PHOTO_1 PA_4
#define R_SPLIT PA_5
#define R_TAB PA_7

void setup(){
pinMode(L_TAB,INPUT_PULLUP);
pinMode(R_TAB,INPUT_PULLUP);
pinMode(L_SPLIT,INPUT_PULLUP);
pinMode(R_SPLIT,INPUT_PULLUP);
pinMode(PHOTO_0,INPUT_PULLUP);
pinMode(PHOTO_1,INPUT_PULLUP);
Serial.begin(9600);
}

void loop(){
  Serial.println("Left Tab // " + String(analogRead(L_TAB)));
  Serial.println("Left Split // " + String(analogRead(L_SPLIT)));
  Serial.println("Left Photo // " + String(analogRead(PHOTO_0)));
  Serial.println("Right Photo // " + String(analogRead(PHOTO_1)));
  Serial.println("Right Split // " + String(analogRead(R_SPLIT)));
  Serial.println("Right Tab // " + String(analogRead(R_TAB)));
  Serial.println("___________________");
  delay(2000);
}

*/