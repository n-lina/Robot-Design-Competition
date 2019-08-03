
#include <Arduino.h>
#include <Servo.h>

#define R_ALIGN PA_0
#define R_DECIDE PA_1
#define PHOTO_1 PA_2 //right
#define PHOTO_0 PA_3 //left
#define L_DECIDE PA_4
#define L_ALIGN PA_5
#define L_MOTOR_FORWARD PB_6
#define L_MOTOR_BACKWARD PB_7
#define R_MOTOR_FORWARD PB_8
#define R_MOTOR_BACKWARD PB_9
#define ARM_SIDES_LIMIT PA12
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
// #define CLAW_UP PA_8
// #define CLAW_DOWN PA_10
// #define TRIGGER PB5
// #define ECHO PB14
// #define CLAW_SERVO PB4
// #define ARM_SERVO PB3
// #define GAUNTLET PA9
// #define ANGLE_START 70 
// #define ANGLE_FINISH 140
#define KP 140 // PID proportion constant // 205 is  too high 
#define KD 11 // PID derivative constant 
#define THRESHOLD 200 // Threshold for being on or off the line
#define DECIDE_THRESHOLD 300
#define ALIGN_THRESHOLD 500
#define SPEED_TUNING 1.8
#define TURN_DELAY_TIME 200
#define DEBOUNCE 1000
#define START_DETECTION 20000
#define ARM_MOTOR_UP PA_6
#define ARM_MOTOR_DOWN PA_7
#define ARM_LEFT PB_1
#define ARM_RIGHT PB_0
#define ARM_SERVO PA8
#define CLAW_SERVO PA9

#define GO true
//#define SENSORS true
//#define MOTORS true
//#define SPLIT_DECIDE true
//#define TURN_IN_PLACE true

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
bool leftDecide;
bool rightDecide;
bool leftAlign;
bool rightAlign;
int debounce = 0;
int junctionNumber = 0;
int loopCounter = 0;
bool pressed = false;
bool done = false;
bool THANOS = true;
Servo clawServo; 
Servo armServo; 

void turnLeft();
void turnRight();
void turnRightAfterPillar(); //after junction 3 thanos 
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();
void alignPillar();
void alignPillarSixTHANOS();
void getPosition();
void collectStone();

void setup()
{
 Serial.begin(9600);
 pinMode(R_ALIGN, INPUT_PULLUP);
 pinMode(R_DECIDE, INPUT_PULLUP);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_DECIDE, INPUT_PULLUP);
 pinMode(L_ALIGN, INPUT_PULLUP);
 pinMode(ARM_SIDES_LIMIT, INPUT_PULLUP); 
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
   pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO); 
  armServo.write(180);
//  if(THANOS){
//    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//  }
//  else if (!THANOS){
//    pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//   }
 }


void loop(){
  if(!done && loopCounter < START_DETECTION){
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
  loopCounter++;
}
else{
  done = true;

  // if(!pressed && digitalRead(ARM_SIDES_LIMIT) == HIGH){
  //   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //   pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //   pressed = true;
  // }

  leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
  rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
  leftDecide = analogRead(L_DECIDE) >= DECIDE_THRESHOLD;
  rightDecide = analogRead(R_DECIDE) >= DECIDE_THRESHOLD;

  if((leftDecide || rightDecide) && (debounce > DEBOUNCE)){
    junctionNumber++;
    debounce = 0;
    switch(junctionNumber){
      case 1: 
        // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0);
        // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0);
        // delay(400);
        stop();
        turnRight();
        // stop();
        // delay(2000);
        break;
      case 2: 
        if(leftDecide){
          turnLeft();
        }
        break;
      case 3:
        // stop();
        // alignPillar();
        // turnRightPillarThree();
        // break;
        pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 350, 0);
        pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 350, 0);
        delay(2000);
        turnInPlaceRight();
        break;

      // case 6:
      //   stop();
      //   alignPillarSixTHANOS();
      //   break;
      case 4: 
        stop();
        alignPillar();
        collectStone();
        break;
      case 5: 
        break;
      case 6: 
        stop();
        turnLeft();
        pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
        pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
        delay(500);
        turnInPlaceRight();
        break;
      case 7: 
        break;
    }
  }

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
  debounce++;
}
}
//}

// void turnInPlaceLeft(){
//   pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
//   pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
//   delay(TURN_DELAY_TIME);
//   while(true){
//     if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
//       stop();
//       delay(50);
//       turnRight();
//       return;
//     }
//   }
// }

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 500, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 500, 0); 
  delay(400);
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

// void turnRightPillarThree(){
//   pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 200, 0); //turn right
//   pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
//   delay(TURN_DELAY_TIME);
//   while(true){
//     if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
//       return;
//     }
//   }
// }

void alignPillar(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 100, 0); //very slow // best case: 70
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 100, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= ALIGN_THRESHOLD || analogRead(R_ALIGN) >= ALIGN_THRESHOLD){
      stop();
      return;
    }
  }
  return;
}

// void alignPillarSixTHANOS(){
//   pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 350, 0); //very slow
//   pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 160, 0);
//   while(true){
//     getPosition();
//     if(analogRead(L_ALIGN) >= DECIDE_THRESHOLD || analogRead(R_ALIGN) >= DECIDE_THRESHOLD){
//       stop();
//       delay(3000);
//       return;
//     }
//   }
//   return;
// }

void getPosition(){
  leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
  rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
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
  lastPosition = position; 
}

void collectStone(){
  clawServo.write(0);
  armServo.write(0);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  clawServo.write(180); //deploying claw
  delay(850); // waiting for it to grasp
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(3500);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  armServo.write(180);
  delay(500);
  //clawServo.write(0);
  //delay(3000);
  return;  
}

#endif

#ifdef SENSORS 
void setup(){
  pinMode(PHOTO_0, INPUT_PULLUP);
  pinMode(PHOTO_1, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
//  Serial.println("Right Align: ");
//  Serial.println(String(analogRead(R_ALIGN)));
 Serial.println("Right Decide: ");
 Serial.println(String(analogRead(R_DECIDE)));
 Serial.println("Right Photo: ");
 Serial.println(String(analogRead(PHOTO_1)));
 Serial.println("Left Photo: ");
 Serial.println(String(analogRead(PHOTO_0)));
 Serial.println("Left Decide: ");
 Serial.println(String(analogRead(L_DECIDE)));
//  Serial.println("Left Align: ");
//  Serial.println(String(analogRead(L_ALIGN)));
 Serial.println("________________");
 delay(2000);
}
#endif


#ifdef MOTORS 

void stop();

void setup(){
pinMode(L_MOTOR_FORWARD, OUTPUT);
pinMode(L_MOTOR_BACKWARD, OUTPUT);
pinMode(R_MOTOR_FORWARD, OUTPUT);
pinMode(R_MOTOR_BACKWARD, OUTPUT);
// pinMode(ARM_LEFT, OUTPUT);
// pinMode(ARM_RIGHT, OUTPUT);
pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
// pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
// pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1); 
Serial.begin(9600);
}

void loop(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  delay(2000);
  stop();
  delay(2000);
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0);
  delay(2000);
  stop();
  delay(2000);

  // pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // delay(2000);
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

  if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
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
    if(digitalRead(ARM_SIDES_LIMIT)==HIGH){
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

