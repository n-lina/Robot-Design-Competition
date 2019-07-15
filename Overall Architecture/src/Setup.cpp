#include <Arduino.h>
#include "Constants.h"
#include "Setup.h"
#include "ManageStone.h"
#include "TapeFollower.h"
#include "TuningMenu.h"

Setup::Setup():
state(0), TEAM(true), stoneNumber(0), splitNumber(0), direction_facing(true), direction(LEFT),
KP_WHEEL (30), KD_WHEEL(9), THRESHOLD(200), SPLIT_THRESHOLD(200), TAB_THRESHOLD(200), 
GAUNTLET_TAPE_THRESHOLD(200), EDGE_THRESHOLD(15), COLLISION_THRESHOLD(5), PILLAR_DISTANCE(3)
{}

static void Setup::setup(){
    
  // Methanos or Thanos
  switch (digitalRead(T_OR_M)){
    case HIGH: 
      TEAM = true;
    case LOW:
      TEAM = false;
  }

  // Instantiating Classes
  ManageStone stoneRobot (stoneNumber, state, direction); 
  TapeFollower tapeRobot (state, stoneNumber, splitNumber, TEAM, direction_facing); 
  TuningMenu tuneRobot; 
  
  PinName outputPins [12] ={ARM_SERVO, ARM_MOTOR_LEFT, ARM_MOTOR_RIGHT, CLAW_SERVO, CLAW_MOTOR_UP, CLAW_MOTOR_DOWN, 
  L_GAUNTLET_SERVO, LEFT_FORWARD_WHEEL_MOTOR, RIGHT_FORWARD_WHEEL_MOTOR, R_GAUNTLET_SERVO, LEFT_BACKWARD_WHEEL_MOTOR, 
  RIGHT_BACKWARD_WHEEL_MOTOR};
  // input_pullup or output

  PinName inputPins [16] = {ARM_SONAR, CLAW_ENCODER, ARM_HOME_SWITCH, TUNING_KNOB_A, TUNING_KNOB_B, TUNING_BUTTON,
  L_TAPE_FOLLOW, R_TAPE_FOLLOW, L_SPLIT, R_SPLIT, L_TAB, R_TAB, L_GAUNTLET, R_GAUNTLET, CALIBRATE, T_OR_M};
  
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

  // Loading calibrated values 
  KP_WHEEL = *tuneRobot.p_KP_WHEEL;
  KD_WHEEL = *tuneRobot.p_KD_WHEEL;
  THRESHOLD = *tuneRobot.p_THRESHOLD;
  SPLIT_THRESHOLD = *tuneRobot.p_SPLIT_THRESHOLD;
  TAB_THRESHOLD = *tuneRobot.p_TAB_THRESHOLD;
  GAUNTLET_TAPE_THRESHOLD = *tuneRobot.p_GAUNTLET_TAPE_THRESHOLD;
  EDGE_THRESHOLD = *tuneRobot.p_EDGE_THRESHOLD;
  COLLISION_THRESHOLD = *tuneRobot.p_COLLISION_THRESHOLD;
  PILLAR_DISTANCE = *tuneRobot.p_PILLAR_DISTANCE;
  }

}


// attach and setup interrupts / interrupt handler 

// put RC filter for switch debouncing 
