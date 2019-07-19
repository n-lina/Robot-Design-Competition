#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(Robot const* robot):
  my_PILLAR_DISTANCE(Robot::instance()->PILLAR_DISTANCE)
  {}

void ManageStone::collectStone(){
  Robot::instance()->stoneNumber++;
  turnClaw();
  moveArmToPillar();
  // if(Robot::instance()->stoneNumber > 2 || Robot::instance()->collisionNumber >1){
  //   raiseClaw();
  // }
  switch(Robot::instance()->stoneNumber){
    case 1: case 2:
    //TODO
    break;
    case 3:
    //TODO
    break;
    case 4:
    //TODO
    break;
  }
  Robot::instance()->clawServo.write(180); //deploy claw to get stone 
  if(digitalRead(NO_STONE) == LOW){ //there is a stone
    if(Robot::instance()->direction){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      return;
    }
    else{
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      return;
    } 
  }
  else{ //no stone 
    return; //dont move arm to centre
  }
}

void ManageStone::dropInStorage(){
  switch(Robot::instance()->stoneNumber){
    case 1: // 6 inches
      Robot::instance()->armServo.write(20);
      break;
    case 2: // 6 inches
      Robot::instance()->armServo.write(40);
      break;
    case 3: // 9 inches
      pwm_start(CLAW_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(NINE_INCH_TIME); //bring down to 6 inches or w.e default height is 
      Robot::instance()->armServo.write(60);
      break;
    case 4: // 12 inches
      pwm_start(CLAW_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(TWELVE_INCH_TIME);
      Robot::instance()->armServo.write(120);
      break;
  } 
  Robot::instance()->clawServo.write(0); //open claw
} 

void ManageStone::moveArmToPillar(){
  if(Robot::instance()->direction){
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  else{
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  while (true){
    if(analogRead(ARM_SONAR)<=my_PILLAR_DISTANCE || digitalRead(ARM_LIMIT) == HIGH){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
      return; 
    }
  }
}

void ManageStone::turnClaw(){
  if(Robot::instance()->direction){
    Robot::instance()->armServo.write(180); //180 degrees corresponds to right side
  }
  else{
    Robot::instance()->armServo.write(0);
  }
}

void ManageStone::raiseClaw(){ //backup function using sonar 
  if(Robot::instance()->collisionNumber != 0){
    pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
    while(true){
      if(analogRead(ARM_SONAR)>=my_PILLAR_DISTANCE+3){ //check that sonar doesnt become negative
        pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
        return;
      }
    }
  }
}

