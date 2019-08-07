#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"
#include "Constants.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono9pt7b.h>

// #define OLED_RESET -1  // Not used
// Adafruit_SSD1306 display(OLED_RESET);

// #if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
// #endif

//#define TUNING true
#define ALL_TOGETHER true
//#define COLLECT_STONE_X true //change X to be what stoneNumber u want, check the direction
// test no stone, choose stoneNumber 1,2, or 3 
//#define COLLECT_STONE_SONAR true
//#define GO_HOME true 
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