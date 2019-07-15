#include <Arduino.h>

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

void setup() {
  // tuning menu function 
  // function for setting up all pins, initiializing functions and interupts 

}
void loop() {
  switch (state){
    case 0: // Go a certain distancce without checking for tabs/splits
      if(splitNumber == 0){
        //goDistance(BOTTOM_DESTINATION, true, CHECKPT_A, CHECKPT_B);        
      }
      else{
        //goDistance(TOP_DESTINATION, false, -1, -1);
      }
    case 1: // Follow Tape checking splits/tabs
    case 2: // Avoid Collision {4}
    // move backwards set distance by rotary encoder and turn in place
    case 3: // Split Chooser 
    case 4: // Collect Stone {3}
    case 5: // Deposit Stones 
    case 6: // Back away from edge {1 - put in interrupt handler}
    case 7: // Go home and deposit stones at certain time (1:30) {2} turn off split/tab interrupts 
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