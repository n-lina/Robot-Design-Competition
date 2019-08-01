
#include <Arduino.h>
#include <Servo.h>

#define L_TAB PA_0
#define L_SPLIT PA_1
#define PHOTO_0 PA_2
#define PHOTO_1 PA_3
#define R_SPLIT PA_4
#define R_TAB PA_5
#define L_MOTOR_FORWARD PB_6
#define L_MOTOR_BACKWARD PB_7
#define R_MOTOR_FORWARD PB_8
#define R_MOTOR_BACKWARD PB_9
#define KP 101 // PID proportion constant // 205 is  too high 
#define KD 11 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define TAB_THRESHOLD 400
#define SPEED_TUNING 1.8
#define TURN_DELAY_TIME 200
#define CLAW_UP PA_8
#define CLAW_DOWN PA_10
#define ARM_LEFT PB_0
#define ARM_RIGHT PB_1
#define PILLAR_DISTANCE 10 //cm 
#define TRIGGER PB5
#define ECHO PB14
#define CLAW_SERVO PB4
#define ARM_SERVO PB3
#define MULTIPLEX_A PA7
#define MULTIPLEX_B PA6
#define MULTIPLEX_C PA12
#define MULTIPLEX_OUT PA14
#define GAUNTLET PA9
#define ANGLE_START 70 
#define ANGLE_FINISH 140

//#define GO true
//#define SENSORS true
//#define MOTORS true
#define SPLIT_DECIDE true
//#define TURN_IN_PLACE true
//#define VOLTAGE true

#ifdef GO
int derivative; 
int timeStep=0; 
int lastError=0; 
int position; 
int lastPosition=0; 
int PID; 
int default_speed = MAX_SPEED/SPEED_TUNING;
int number=0;
bool leftSensor;
bool rightSensor;
bool leftTab;
bool rightTab;
bool pressed = false;
bool THANOS = true;

void turnLeft();
void turnRight();
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();
bool multi(bool C, bool B, bool A);

void setup()
{
 Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_MOTOR_BACKWARD, OUTPUT);
 pinMode(L_MOTOR_FORWARD, OUTPUT);
 pinMode(R_MOTOR_BACKWARD, OUTPUT);
 pinMode(R_MOTOR_FORWARD, OUTPUT);
 pinMode(ARM_LEFT, OUTPUT);
 pinMode(ARM_RIGHT, OUTPUT);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
 pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
 if(THANOS){
   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 }
 else if (!THANOS){
   pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
}
 }


void loop(){
//  leftTab = analogRead(L_TAB) >= THRESHOLD; 
//  rightTab = analogRead(R_TAB) >= THRESHOLD; 

//  if((leftTab || rightTab) && !pressed){
//    turnInPlaceRight();
//    pressed =  true;
//  }


Serial.println(multi(1,0,0));
// if(!pressed && multi(1,0,0) == HIGH){
//   pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
//   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
//   pressed = true;
// }

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

 if(PID>(default_speed)){
   PID = default_speed;
 }

 else if(PID<-default_speed){
   PID = -default_speed;
 }

 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0); 
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

void turnInPlaceLeft(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
}

void stop(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
}

void turnLeft(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void turnRight(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

bool multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C); 
  return digitalRead(MULTIPLEX_OUT);
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
  // Serial.println("Right // ");
  // Serial.println(String(analogRead(PHOTO_1)));
  // Serial.println("Left // ");
  // Serial.println(String(analogRead(PHOTO_0)));
  // Serial.println("Left Tab: ");
  // Serial.println(String(analogRead(L_TAB)));
  // Serial.println("Right Tab: ");
  // Serial.println(String(analogRead(R_TAB)));
  // Serial.println("________________");
  delay(1000);
}
#endif


#ifdef MOTORS 

void stop();

void setup(){
pinMode(L_MOTOR_FORWARD, OUTPUT);
pinMode(L_MOTOR_BACKWARD, OUTPUT);
pinMode(R_MOTOR_FORWARD, OUTPUT);
pinMode(R_MOTOR_BACKWARD, OUTPUT);
pinMode(ARM_LEFT, OUTPUT);
pinMode(ARM_RIGHT, OUTPUT);
pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1); 
Serial.begin(9600);
}

void loop(){
  // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  //delay(2000);
  // pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
  // delay(1000);
  // pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // delay(2000);
  // pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
}

void stop(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
}
#endif

#ifdef SPLIT_DECIDE
bool THANOS = false; 
int derivative; 
int timeStep=0; 
int position; 
int lastPosition=0; 
int PID; 
int number;
bool leftSensor;
bool rightSensor;
bool leftTab; 
bool rightTab;
bool leftSplit; 
bool rightSplit;
bool pressed = false;
int loopCounter =0;
int splitNumber =0;
int default_speed = MAX_SPEED/SPEED_TUNING;

void turnLeft();
void turnRight();
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();
void collectStone();
bool multi(bool C, bool B, bool A);
void dropGauntlet();

Servo armServo; 
Servo clawServo; 
Servo gauntletServo;

void setup()
{
 Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_SPLIT, INPUT_PULLUP);
 pinMode(R_SPLIT, INPUT_PULLUP);
  pinMode(CLAW_UP, OUTPUT);
  pinMode(CLAW_DOWN, OUTPUT);
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(MULTIPLEX_C, OUTPUT);
  pinMode(MULTIPLEX_OUT, INPUT);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO); 
  gauntletServo.attach(GAUNTLET);
  pinMode(GAUNTLET, OUTPUT);
  gauntletServo.attach(GAUNTLET);
 splitNumber = 0;
 gauntletServo.write(90);
 
 if(THANOS){
   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 }
 else{
   pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 }
}

void loop(){
  leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
  rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
  leftSplit =  analogRead(L_SPLIT)>=TAB_THRESHOLD;
  rightSplit = analogRead(R_SPLIT)>=TAB_THRESHOLD;
  
  if((leftSplit || rightSplit) && (rightSensor || leftSensor)){
    splitNumber++;
    switch(splitNumber){
      case 1: case 2:
        if(THANOS){
          turnLeft();
        }
        else{
          turnRight();
        }
        break;
      case 3: case 4: case 5: case 6: case 7: case 8:
        break; 
      case 9: 
        delay(50);
        stop();
        collectStone();
        delay(5000);
        dropGauntlet();
        break;
    }
  }

  if(!pressed && multi(1,0,0)==HIGH){
    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pressed = true;
  }
   
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
 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0);

 if(lastPosition != position){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
  }
 lastPosition = position;
}


void turnLeft(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void turnRight(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void stop(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  return;
}


void turnInPlaceLeft(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
}

void collectStone(){
  clawServo.write(90);
  armServo.write(0);
 // pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  while(true){
    if(multi(1,0,0)==HIGH){
      pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      break;
    }
  }
  clawServo.write(180); 
  delay(1000);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  armServo.write(90);
  delay(1000);
  clawServo.write(0);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3000);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  return;
}

bool multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C); 
  return digitalRead(MULTIPLEX_OUT);
}

void dropGauntlet(){
  for (int i = ANGLE_START; i <= ANGLE_FINISH; i = i + 3) {
    gauntletServo.write(i);
    delay(15);
  }
  delay(1000);

  for (int i = ANGLE_START; i >= ANGLE_FINISH; i = i - 3) {
    gauntletServo.write(i);
    delay(15);
  }
  delay(1000);
}

#endif

#ifdef TURN_IN_PLACE
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();

void setup(){
  pinMode(L_MOTOR_FORWARD, OUTPUT);
  pinMode(R_MOTOR_FORWARD, OUTPUT);
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
  pinMode(L_MOTOR_BACKWARD, OUTPUT);
  pinMode(R_MOTOR_BACKWARD, OUTPUT);
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
}

void loop(){
  turnInPlaceLeft();
  stop();
  delay(3000);
}

void turnInPlaceLeft(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
}

void stop(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
}
#endif

#ifdef VOLTAGE
void setup(){
  Serial.begin(9600);
  pinMode(MULTIPLEX_OUT, INPUT);
}

void loop() {
  Serial.println(digitalRead(MULTIPLEX_OUT));
}

#endif