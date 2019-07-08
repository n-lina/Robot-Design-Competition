
#include <Arduino.h>

//make KP and KD adjustable by potentiometer? 

#define PHOTO_0 PA_3
#define PHOTO_1 PA_4
#define L_MOTOR_FORWARD PA_6
//#define L_MOTOR_BACKWARD PA_2
#define R_MOTOR_FORWARD PB_0
//#define R_MOTOR_BACKWARD PB_1
#define KP 28 // PID proportion constant // 205 is  too high 
#define KD 9 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define SETPOINT 0 //Position 0 when sensor 3 has the lowest reading 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define TUNING 0.3
 
int error; 
int derivative; 
int timeStep=0; 
int lastError=0; 
int position; 
int lastPosition=0; 
int PID; 
int number;
int leftSensor;
int rightSensor;

void setup()
{
 //Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 //pinMode(L_MOTOR_BACKWARD, INPUT_PULLDOWN);
 pinMode(L_MOTOR_FORWARD, INPUT_PULLDOWN);
 //pinMode(R_MOTOR_BACKWARD, INPUT_PULLDOWN);
 pinMode(R_MOTOR_FORWARD, INPUT_PULLDOWN);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
}

void loop()
{
 leftSensor = analogRead(PHOTO_0);
 rightSensor = analogRead(PHOTO_1);
 timeStep++;

 if (leftSensor >= THRESHOLD && rightSensor >= THRESHOLD){
   position = 0;
 } 
 else if(leftSensor >= THRESHOLD && rightSensor < THRESHOLD){
   position = 1; 
 }
 else if(leftSensor < THRESHOLD && rightSensor >= THRESHOLD){
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

  lastPosition = position; 

 // error is opposite sign of position. 
 // L on, R off : position = positive, error = negative, want to turn LEFT 
 // R on, L off : position = negative, error = positive, want to turn RIGHT 
 error = SETPOINT-position; 

 derivative = (error - lastError) / timeStep; 
 // Serial.println("Derivative: // "+ (String)derivative);

 // PID output will be same sign as the error 
 PID = (KP * error) + (KD * derivative); 
 // Serial.println("PID: // " + (String)PID);
 // Serial.println("                  ");

 // CALCULATING WHAT KP TO USE
 // 1024/5 = 204.8 
 // Setting the MIN_INPUT into the pwm_start to be 50, the new kp is calculated to be: 
 // (1024-50)/5 = 195
 // instead of having IF statements that check if input into pwm < MIN_INPUT which will slow it down 


 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)-PID, 0);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)+PID, 0); 

 if(lastError != error){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
 }
 lastError = error;

}

