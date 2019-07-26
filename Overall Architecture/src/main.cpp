#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"
#include "Constants.h"


//#include <Setup.h>
  // declare functions and interrupts, interrupt handlers

//#define ALL_TOGETHER true
#define COLLECT_STONE_X true //change X to be what stoneNumber u want, check the direction
// test no stone, choose stoneNumber 1,2, or 3 
//#define COLLECT_STONE_SONAR true
//#define GO_HOME
//#define MULTIPLEX

#ifdef ALL_TOGETHER 
TapeFollower robot(Robot::instance());
ManageStone Robot(Robot::instance());

void setup() {
 Robot::instance()->setup();
 Serial.begin(9600);
}

void loop() {
  switch (Robot::instance()->state){
    case GO_DISTANCE: // Go a certain distance without checking for tabs/splits
      //TODO
      break;
    case FOLLOW_TAPE: // Follow Tape checking splits/tabs
      robot.followTape();
      break;
    case AVOID_COLLISION: // Avoid Collision {4}
      //TODO // no more collision sonar 
      break; 
    case SPLIT_CHOOSER: // Split Chooser 
      robot.splitDecide();
      break;
    case COLLECT_STONE: // Collect Stone {3}
      Robot.collectStone();
      break;
    case GO_HOME: // Go home and deposit stones at certain time (1:30) {2} turn off collision interrupts 
      robot.goHome();
      break;
    case PARK: // park at gauntlet
      robot.park();
  }
}
#endif

#ifdef COLLECT_STONE_X
ManageStone Robot(Robot::instance());

void setup() {
  Serial.begin(9600);
  Robot::instance()->setup();
  Robot::instance()->direction = LEFT;
  Robot.collectStone();
}

void loop (){
}
#endif

#ifdef GO_HOME
TapeFollower robot(Robot::instance());

void setup() {
 pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
 pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
 pinMode(LEFT_FORWARD_WHEEL_MOTOR, OUTPUT);
 pinMode(RIGHT_FORWARD_WHEEL_MOTOR, OUTPUT);
 pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);   
 Serial.begin(9600);
 robot.goHome(true);
}

void loop() {
}
#endif

#ifdef MULTIPLEX 
int curr_val;
bool multi(bool A, bool B, bool C);

void setup(){
  Serial.begin(9600);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(MULTIPLEX_C, OUTPUT);
  pinMode(MULTIPLEX_OUT, INPUT);
}

void loop(){
  //Select each pin and read value
  curr_val = multi(1, 0, 0);
  Serial.println(curr_val);
  delay(2000);
}

bool multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C);

  bool multi_out = digitalRead(MULTIPLEX_OUT);  
  return multi_out;
}

#endif 