
#include <Arduino.h>

#define PHOTO_0 PA_2
#define PHOTO_1 PA_3

#define L_MOTOR_FORWARD PB_1
//#define L_MOTOR_BACKWARD PB_0
#define R_MOTOR_FORWARD PA_6
//#define R_MOTOR_BACKWARD PA_7
#define KP 130
#define KD 10
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 
#define THRESHOLD 200 
#define TUNING 1.5

//#define GO true
//#define SENSORS true
#define MOTORS true

#ifdef GO
int derivative; 
int timeStep=0; 
int lastError=0; 
int position; 
int lastPosition=0; 
int PID; 
int number=0;
bool leftSensor;
bool rightSensor;

void setup()
{
 //Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 //pinMode(L_MOTOR_BACKWARD, OUTPUT);
 pinMode(L_MOTOR_FORWARD, OUTPUT);
 //pinMode(R_MOTOR_BACKWARD, OUPUT);
 pinMode(R_MOTOR_FORWARD, OUTPUT);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
}


void loop()
{
 leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
 rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
 timeStep++;

 if (leftSensor  && rightSensor ){
   position = 0;
 } 
 else if(leftSensor  && !rightSensor ){
   position = 1; 
 }
 else if(!leftSensor && rightSensor ){
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

 // L on, R off : position = positive, want to turn LEFT 
 // R on, L off : position = negative, want to turn RIGHT 

  derivative = (position - lastPosition) / timeStep; 
  PID = (KP * position) + (KD * derivative); 

 // CALCULATING WHAT KP TO USE
 // 1024/5 = 204.8 
 // Setting the MIN_INPUT into the pwm_start to be 50, the new kp is calculated to be: 
 // (1024-50)/5 = 195
 // instead of having IF statements that check if input into pwm < MIN_INPUT which will slow it down 
 
 if(PID>(MAX_SPEED/TUNING)){
   PID = MAX_SPEED/TUNING;
 }

 else if(PID<-(MAX_SPEED/TUNING)){
   PID = -MAX_SPEED/TUNING;
 }

 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/TUNING)+PID, 0);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/TUNING)-PID, 0); 
 //input of 18 still runs

 if(lastPosition != position){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
 }
 lastPosition = position; 

}
#endif

#ifdef SENSORS 
void setup(){
  pinMode(PHOTO_0, INPUT_PULLUP);
  pinMode(PHOTO_1, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
  //Serial.println("success");
  Serial.println("Right // ");
  Serial.println(String(analogRead(PHOTO_1)));
  Serial.println("Left // ");
  Serial.println(String(analogRead(PHOTO_0)));
  Serial.println("________________");
  delay(1000);
}
#endif


#ifdef MOTORS
void setup(){
  Serial.begin(9600);
 //pinMode(L_MOTOR_BACKWARD, OUTPUT);
 pinMode(L_MOTOR_FORWARD, OUTPUT);
 //pinMode(R_MOTOR_BACKWARD, OUPUT);
 pinMode(R_MOTOR_FORWARD, OUTPUT);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
 
 Serial.println("hello");
}

void loop(){
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/TUNING), 0);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/TUNING), 0); 
 Serial.println("______________________");
 Serial.println(String(MAX_SPEED/TUNING));
 //delay(1000);
}
#endif

