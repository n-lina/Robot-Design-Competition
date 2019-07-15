#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(int* stoneNumber, int* state,  bool* direction):
  _stoneNumber(stoneNumber), _state(state), _direction(direction), lastEncoderState(0), encoderState(0), clawHeight(0), armServo(), 
  clawServo(), motor(0)
  {}

void ManageStone::collectStone(){
  turnClaw();
  moveArmToPillar();
  raiseClaw();
  clawServo.write(180); //deploy claw to get stone 
  _stoneNumber++;
}
void ManageStone::dropInStorage(){
  moveArmToCentre();
  armServo.write(90);
  lowerClaw();
  switch(_stoneNumber){
    case 1: 
      armServo.write(20);
      break;
    case 2:
      armServo.write(40);
      break;
    case 3:
      armServo.write(60);
      break;
    case 4: 
      armServo.write(120);
      break;
    case 5: 
      armServo.write(140);
      break;
    case 6: 
      armServo.write(160);
      break;
  } 
  clawServo.write(0); //open claw
  armServo.write(90); //return to default position 
} 

void ManageStone::moveArmToPillar(){
  switch (_direction){
    case LEFT: 
      motor = ARM_MOTOR_LEFT;
    case RIGHT: 
      motor = ARM_MOTOR_RIGHT;
  }
  pwm_start(motor, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  while (true){
    if(analogRead(ARM_SONAR)<=PILLAR_DISTANCE){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
      return; 
    }
  }
}

void ManageStone::moveArmToCentre(){
  switch(_direction){
    case LEFT: 
      motor = ARM_MOTOR_RIGHT;
    case RIGHT: 
      motor = ARM_MOTOR_LEFT;
    } 
  pwm_start(motor, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); 
  while (true){
    if(digitalRead(ARM_HOME_SWITCH)==HIGH){ //how does the microswitch work 
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
      return; 
    }
  }
}

void ManageStone::turnClaw(){
  if(_direction){
    armServo.write(180); //180 degrees corresponds to right side
  }
  else{
    armServo.write(0);
  }
}

void ManageStone::raiseClaw(){
  pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  while(true){
    if(analogRead(ARM_SONAR)>=PILLAR_DISTANCE+3){ //check that sonar doesnt become negative
      pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
      return;
    }
  }
}
//raising claw
    //if there is a delay in stopping, then we dont need to go extra x distance using rotary encoder 
    //if there is no delay in stopping, ie. it stops exactly when the sonar reading jumps, 
    //use rotary encoder to move up a bit more.

void ManageStone::lowerClaw(){
  pwm_start(CLAW_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  while(true){
    if(clawHeight <= 0){
      pwm_start(CLAW_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
      return;
    }
  }
}

//use interrupts or find better way to count height for raiseClaw() and lowerClaw() since rn 
//with just a counter, it could miss counts if the motor is too fast :|

