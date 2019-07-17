#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"

//#include <Constants.h>
  // pins 
  // PID values 
  // PWN values 
  // sensor thresholds 
  // max and min vals 
  // global variables 
//#include <FollowTape.h>
  // turn in place
  // go left 
  // go right 
  // public split number variable 
  // follow tape 
  // poll main PTs
  // split or tab (bool?)
  // stop?
  // go home
//#include <CollectStone.h>
  // raise/lower claw 
  // turn claw x degrees
  // close / open claw 
  // move arm left/right 
  // lower drawbridge
  // put stone in temp storage ( keep track of stone # )
//#include <Setup.h>
  // initialize sensors and motors with PWM 
  // startup menu 
  // OLED setup 
  // declare functions and interrupts, interrupt handlers

  TapeFollower robot(Robot::instance());
  ManageStone Robot(Robot::instance());

void setup() {
  Robot::instance()->setup();  
}

void loop() {
  switch (Robot::instance()->state){
    case GO_DISTANCE: // Go a certain distancce without checking for tabs/splits
      break;
    case FOLLOW_TAPE: // Follow Tape checking splits/tabs
      break;
    case AVOID_COLLISION: // Avoid Collision {4}
    // move backwards set distance by rotary encoder and turn in place
      break; 
    case SPLIT_CHOOSER: // Split Chooser 
      break;
    case COLLECT_STONE: // Collect Stone {3}
      break;
    case BACK_AWAY_FROM_EDGE: // Back away from edge {1 - put in interrupt handler}
      break;
    case GO_HOME: // Go home and deposit stones at certain time (1:30) {2} turn off collision interrupts 
      break;
    case PARK: // park at gauntlet
      break;
  }
}

// 0. start coding functions needed
// 1. organize code into libraries and headers 
// 2. code interrupts 
// 3. make timer interupts ?

/* 

EXTERNAL INTERRUPTS 
Collsion - microswitches and both sonars 
Edge - sonar 
Rotary Encoders for 2 wheels, claw height 

TIMER INTERRUPTS
Split 
Tab 

GITHUB
1. select directory 
2. git bash here 
3. clone repository into that folder 
4. commit onto github 

*/