#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"
#include "Constants.h"


//#define TUNING true
//#define ALL_TOGETHER true
//#define COLLECT_STONE_X true //change X to be what stoneNumber u want, check the direction
// test no stone, choose stoneNumber 1,2, or 3 
//#define COLLECT_STONE_SONAR true
//#define GO_HOME true 
#define MULTIPLEX true 

#ifdef ALL_TOGETHER 
TapeFollower robot(Robot::instance());
ManageStone Robot(Robot::instance());

void setup() {
  //Serial.begin(9600);
  Robot::instance()->setup();
}

void loop() {
  switch (Robot::instance()->state){
    case GO_DISTANCE: // Go a certain distance without checking for tabs/splits
      robot.goDistance(START_DETECTION);
      break;
    case FOLLOW_TAPE: // Follow Tape checking splits/tabs
      robot.followTape();
      break;
    case AVOID_COLLISION: // Avoid Collision {4}
      robot.avoidCollision();
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
    case TURN_IN_PLACE_RIGHT:
      robot.turnInPlaceRight();
      break; 
    case TURN_IN_PLACE_LEFT:
      robot.turnInPlaceLeft();
      break;
    case PARK: // park at gauntlet
      robot.dropGauntlet();
      robot.park();
      break;
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
  Robot::instance()->setup();
}

void loop() {
  robot.goHome(true);
}
#endif

#ifdef TUNING 
void setup(){
  Serial.begin(9600);  
}
void loop(){
  Serial.println("in loop");
  Robot::instance()->setup();
  // int o = 1;
  // Serial.println(o);
  // int* p = (int*)0x0800FFF3;
  // int &o = (int*)0x0800FFF3;
  // //int* p = (int*)134283251;
  // //*p =  0x1234;
  // //Serial.println((unsigned int)p);
  // Serial.println(*p);

  // //Robot::instance()->setup();
  // delay(1000);
}
#endif

#ifdef MULTIPLEX
 
 Servo gauntlet;
void setup(){
  //Robot::instance()->setup();
    pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP); 
  Serial.begin(9600);

}
void loop(){
  Serial.println("R Align: " + String(analogRead(R_ALIGN)));
  Serial.println("R Decide: " + String(analogRead(R_DECIDE)));
  Serial.println("R TapeFollow: " +String(analogRead(R_TAPE_FOLLOW)));
  Serial.println("L TapeFollow: " +String(analogRead(L_TAPE_FOLLOW)));
  Serial.println("L Decide: " + String(analogRead(L_DECIDE)));
  Serial.println("L Align: " + String(analogRead(L_ALIGN)));
  Serial.println("___________________________");

  // Robot::instance()->clawServo.write(0);
  // delay(1000);



  
  // Robot::instance()->clawServo.write(180);
  // delay(1000);
}
#endif