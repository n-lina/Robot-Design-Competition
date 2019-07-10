
#include <Arduino.h>

//make KP and KD adjustable by potentiometer? 

#define L_SPLIT PA_1
#define PHOTO_0 PA_3
#define PHOTO_1 PA_4
#define R_SPLIT PA_5
#define L_MOTOR_FORWARD PA_6
//#define L_MOTOR_BACKWARD PA_2
#define R_MOTOR_FORWARD PB_0
//#define R_MOTOR_BACKWARD PB_1
#define KP 26 // PID proportion constant // 205 is  too high 
#define KD 9 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define SPLIT_THRESHOLD 500
 
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

int loopCounter =0;

void setup()
{
 Serial.begin(9600);
 //Serial.println("hello");
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_SPLIT, INPUT_PULLUP);
 pinMode(R_SPLIT, INPUT_PULLUP);
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

 //pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+PID, 0);
 //pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-PID, 0); 

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
   int leftSplitSensor = analogRead(L_SPLIT);
   int rightSplitSensor = analogRead(R_SPLIT);
   leftSplit = leftSplitSensor >=SPLIT_THRESHOLD;
   rightSplit = rightSplitSensor >=SPLIT_THRESHOLD;
   if(leftSplit || rightSplit){
      Serial.println("split // left" + String(leftSplitSensor) + " // right"+ String(rightSplitSensor));
     // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 40, 0);
     // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 40, 0); 
      delay(1000);
    }
    loopCounter =0;
  }

}

