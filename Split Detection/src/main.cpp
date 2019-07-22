
#include <Arduino.h>


//make KP and KD adjustable by potentiometer? 

#define L_TAB PA_0
#define L_SPLIT PA_1 //2
#define PHOTO_0 PA_2
#define PHOTO_1 PA_3
#define R_SPLIT PA_4
#define R_TAB PA_5
//#define ALIGN_LEFT_TAB PA_5 //pin 
//#define ALIGN_RIGHT_TAB PA_4 //pin
#define L_MOTOR_FORWARD PB_1
//#define L_MOTOR_BACKWARD PB_0
#define R_MOTOR_FORWARD PA_6
//#define R_MOTOR_BACKWARD PA_7
#define KP 162 // PID proportion constant // 205 is  too high 
#define KD 12 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define SPLIT_THRESHOLD 200
#define TAB_THRESHOLD 200
#define SPEED_TUNING 1.2
#define TIME 50
 
#define GO true
//#define SENSORS true

#ifdef GO
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
int splitNumber =0;
int default_speed = MAX_SPEED/SPEED_TUNING;

void alignLeftTab();
void alignRightTab();
void turnLeft();
void turnRight();
void stop();
void goStraight();
void turnLeftSoft();
void turnRightSoft();

void setup()
{
 Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_SPLIT, INPUT_PULLUP);
 pinMode(R_SPLIT, INPUT_PULLUP);
 pinMode(L_TAB, INPUT_PULLUP);
 pinMode(R_TAB, INPUT_PULLUP);
 //pinMode(ALIGN_LEFT_TAB, INPUT_PULLUP);
 //pinMode(ALIGN_RIGHT_TAB, INPUT_PULLUP);
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
 
  if(PID>(default_speed)){
   PID = default_speed;
 }

 else if(PID<-(default_speed)){
   PID = -default_speed;
 }

 //pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0); 
 //pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0);

 if(lastPosition != position){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
 }
 lastPosition = position;
 
   leftSplit = analogRead(L_SPLIT) >= SPLIT_THRESHOLD;
   rightSplit = analogRead(R_SPLIT) >= SPLIT_THRESHOLD;
   leftTab =  analogRead(L_TAB)>=TAB_THRESHOLD;
   rightTab = analogRead(R_TAB)>=TAB_THRESHOLD;
   
   if((leftSplit || rightSplit) && (!leftTab && !rightTab) && (leftSensor || rightSensor)){
    splitNumber++;
    switch(splitNumber){
      case 1:
        turnRight();
        break;
      case 2:
        turnLeftSoft();
        delay(1000);
        splitNumber = 0;
        break;
    }
  }

  else if((leftSplit && leftTab) && (!rightSplit && !rightTab) && (rightSensor || leftSensor)){
    alignLeftTab();
  }

  else if((rightSplit && rightTab) && (!leftSplit && !leftTab) && (rightSensor || leftSensor)){
    alignRightTab();
  }  
}


void turnLeft(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  return;
}

void turnRight(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  return;
}

void turnRightSoft(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 450, 0); 
  return;
}

void turnLeftSoft(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 450, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  return;
}

void stop(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  return;
}

void goStraight(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  return;
}

void alignLeftTab(){
  turnLeft();
  delay(TIME);
  turnRight();
  delay(TIME);
  goStraight();
  delay(TIME);
  stop();
  delay(3000);
  turnRight();
  return;
  /*
  while(true){
    if(analogRead(ALIGN_LEFT_TAB) >= THRESHOLD){
      delay(100);
      stop();
      delay(100);
      return;
    }
  }
  */
}

void alignRightTab(){
  turnRight();
  delay(TIME);
  turnLeft();
  delay(TIME);
  goStraight();
  delay(TIME);
  stop();
  delay(3000);
  turnLeft();
  return;
  
  /*
  while(true){
    if(analogRead(ALIGN_RIGHT_TAB) >= THRESHOLD){
      delay(100);
      stop();
      delay(100);
      return;
    }
  }
  */
}

#endif

#ifdef SENSORS

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

#endif