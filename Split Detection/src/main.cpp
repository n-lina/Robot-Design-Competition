
#include <Arduino.h>
#include <Servo.h>


//TRY:
// slower
// pwm_start every other loop
// split/tab thresholds 
// tune kp/kd

#define R_ALIGN PA_0
#define R_DECIDE PA_1
#define PHOTO_1 PA_2
#define PHOTO_0 PA_3
#define L_DECIDE PA_4
#define L_ALIGN PA_5
#define L_MOTOR_FORWARD PB_6
#define L_MOTOR_BACKWARD PB_7
#define R_MOTOR_FORWARD PB_8
#define R_MOTOR_BACKWARD PB_9
#define KP 140 // PID proportion constant // 205 is  too high 
#define KD 11 // PID derivative constant 
#define MAX_SPEED 1024 // max number the Arduino PWM takes 
#define CLOCK_FQ 100000 //For pwm_start function
#define THRESHOLD 200 // Threshold for being on or off the line
#define DECIDE_THRESHOLD 300
#define SPEED_TUNING 1.8
#define TURN_DELAY_TIME 200
#define ARM_MOTOR_UP PA_6
#define ARM_MOTOR_DOWN PA_7
#define ARM_LEFT PB_1
#define ARM_RIGHT PB_0
// #define PILLAR_DISTANCE 10 //cm 
// #define TRIGGER PB5
// #define ECHO PB14
#define ARM_SERVO PA8
#define CLAW_SERVO PA9
#define ARM_SIDES_LIMIT PA11
// #define GAUNTLET PA9
// #define ANGLE_START 70 
// #define ANGLE_FINISH 140

//#define GO true //not hardcoded
//#define STONE true 
//#define SERVOS true
#define TEST true 

#ifdef GO
bool THANOS = false; 
int derivative; 
int timeStep=0; 
int position; 
int lastPosition=0; 
int PID; 
int number;
bool leftSensor;
bool rightSensor;
bool leftDecide; 
bool rightDecide;
bool leftAlign; 
bool rightAlign;
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
//void dropGauntlet();
void alignPillar();

Servo armServo; 
Servo clawServo; 
Servo gauntletServo;

void setup()
{
 Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_DECIDE, INPUT_PULLUP);
 pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(CLAW_UP, OUTPUT);
  pinMode(CLAW_DOWN, OUTPUT);
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  // pinMode(TRIGGER, OUTPUT);
  // pinMode(ECHO, INPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(CLAW_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO); 
  // gauntletServo.attach(GAUNTLET);
  // pinMode(GAUNTLET, OUTPUT);
  // gauntletServo.attach(GAUNTLET);
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
  leftDecide =  analogRead(L_DECIDE)>=DECIDE_THRESHOLD;
  rightDecide = analogRead(R_DECIDE)>=DECIDE_THRESHOLD;
  
  if((leftDecide || rightDecide) && (rightSensor || leftSensor)){
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
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 500, 0); 
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
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 800, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 800, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 800, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 800, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void collectStone(){
  clawServo.write(90);
  armServo.write(0);
 // pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 //pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // while(true){
  //   if(digitalRead(ARM_SIDES_LIMIT)==HIGH){
  //     pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     break;
  //   }
  // }
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(1500);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  clawServo.write(180); //deploying claw
  delay(1000); // waiting for it to grasp
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(3500);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  dropInStorage();
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // returning to height b4 lifting stone 
  delay(3500);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  dropInStorage();
  return;
}

// void dropGauntlet(){
//   for (int i = ANGLE_START; i <= ANGLE_FINISH; i = i + 3) {
//     gauntletServo.write(i);
//     delay(15);
//   }
//   delay(1000);

//   for (int i = ANGLE_START; i >= ANGLE_FINISH; i = i - 3) {
//     gauntletServo.write(i);
//     delay(15);
//   }
//   delay(1000);
// }

#endif

#ifdef STONE

Servo clawServo; 
Servo armServo; 
//void moveArmToPillar();
//int readSonar();

void setup(){
  Serial.begin(9600);
  pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  // pinMode(TRIGGER, OUTPUT);
  // pinMode(ECHO, INPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO); 
}

void loop(){
  clawServo.write(0);
  armServo.write(0);
// pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
// pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  // while(true){
  //   if(multi(1,0,0)==HIGH){
  //     pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     break;
  //   }
  // }
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  clawServo.write(180); //deploying claw
  delay(850); // waiting for it to grasp
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(3500);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  armServo.write(90);
  delay(500);
  clawServo.write(0);
  delay(3000);
  // pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // returning to height b4 lifting stone 
  // delay(3500);
  // pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
 }

// void moveArmToPillar(){
//   armServo.write(180);
//   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//   while (true){
//     int num = readSonar();
//     if(num<=PILLAR_DISTANCE){ //multi(0,0,1) = limit switch
//       pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
//       pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
//       return; 
//     }
//   }
// }

// int readSonar(){ //CM
//   digitalWrite(TRIGGER, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIGGER, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIGGER, LOW);
//   return pulseIn(ECHO, HIGH)*(0.034/2);
// }


#endif

#ifdef SERVOS 
  Servo armServo; 
  Servo clawServo;

  void setup(){
    pinMode(ARM_SERVO, OUTPUT);
    pinMode(CLAW_SERVO, OUTPUT);
    pinMode(ARM_LEFT, OUTPUT);
    pinMode(ARM_RIGHT, OUTPUT);
    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
    pinMode(ARM_MOTOR_UP, OUTPUT);
    pinMode(ARM_MOTOR_DOWN, OUTPUT);
    pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1);
    armServo.attach(ARM_SERVO);
    clawServo.attach(CLAW_SERVO);
  }
  void loop(){
    armServo.write(0); //180 is left
    delay(1000);
    armServo.write(180);
    delay(1000);
  }
#endif


