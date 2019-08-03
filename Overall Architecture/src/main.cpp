#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"
#include "Constants.h"

#include <Adafruit_SSD1306.h>
#include <FreeMono12pt7b.h>
#include <Wire.h>


//#define ALL_TOGETHER true
//#define COLLECT_STONE_X true //change X to be what stoneNumber u want, check the direction
// test no stone, choose stoneNumber 1,2, or 3 
//#define GO_HOME true 
#define TESTING

Adafruit_SSD1306 display1(-1);

#ifdef ALL_TOGETHER 
TapeFollower robot(Robot::instance());
ManageStone Robot(Robot::instance());

void setup() {
 Robot::instance()->setup();
 Serial.begin(9600);
 Robot::instance()->state = GO_DISTANCE;
 Robot::instance()->TEAM = METHANOS;
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

#ifdef TESTING
void setup(){
  Serial.begin(9600);
  Serial.println("setup");
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  //Robot::instance()->setup();
}
void loop(){
  Serial.println("hello");
  display1.clearDisplay();
  display1.setCursor(2,2);
  display1.println("TESTING");
  display1.display();
  //display1.clearDisplay();
  delay(2000);
  // Robot::instance()->display.clearDisplay();
  // Robot::instance()->display.setCursor(2,2);
  // Robot::instance()->display.print("TESTING");
  // Robot::instance()->display.display();
  // Robot::instance()->display.setCursor(0,15);
  // Robot::instance()->display.print("HELLO");
  // Robot::instance()->display.display();  
}
#endif
