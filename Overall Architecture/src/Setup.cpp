#include <Arduino.h>
#include <Constants.h>
#include <Setup.h>
#include <ManageStone.h>
#include <TapeFollower.h>
#include <TuningMenu.h>

Setup::Setup(){}

static void Setup::setup(){
    
  // Methanos or Thanos
  switch (digitalRead(T_OR_M)){
    case HIGH: 
      TEAM = true;
    case LOW:
      TEAM = false;
  }

  // Instantiating Classes
  ManageStone stoneRobot; 
  TapeFollower tapeRobot; 
  TuningMenu tuneRobot; 

  Servo servos [4] = {stoneRobot.armServo, stoneRobot.clawServo, tapeRobot.L_GauntletServo, 
  tapeRobot.R_GauntletServo};
  
  // Setting up pins
  for (volatile int i=0; i<12; i++){
    pinMode(outputPins[i], OUTPUT);
    pinMode(inputPins[i], INPUT_PULLUP);
    if((i%3)==0){
      servos[i/3].attach(outputPins[i]);
    }
    else{
      pwm_start(outputPins[i], CLOCK_FQ, MAX_SPEED, 0, 1);      
    }
  }
  for (volatile int j=12; j<18; j++){
      pinMode(inputPins[j], INPUT_PULLUP);
  }
}

// put RC filter for switch debouncing 
