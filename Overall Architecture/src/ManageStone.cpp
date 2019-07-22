#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(Robot const* robot):
  my_PILLAR_DISTANCE(Robot::instance()->PILLAR_DISTANCE), my_TEAM(Robot::instance()->TEAM)
  {}

void ManageStone::collectStone(){
  Robot::instance()->stoneNumber++;
  switch(Robot::instance()->stoneNumber){
    case 1:// 6 inches
    moveArmToPillar();
    break;
    case 2: // 6 inches 
    dropInStorage();
    turnClaw();
    moveArmToPillar(); //dropInStorage sets correct height, dont need to move claw up 
    break;
    case 3: // 9 inches
    dropInStorage();
    moveArmToPillar(); //dropInStorage sets correct height, dont need to move claw up 
    break;
    case 4: // 12 inches 
    dropInStorage(); // dropping 3rd stone in storage 
    turnClaw();
    moveArmToPillar();
    Robot::instance()->stoneNumber++;
    dropInStorage(); // dropping 4th stone - no need to translate back to the middle 
    break;
  }
  Robot::instance()->clawServo.write(180); //deploy claw to get stone 
  if(digitalRead(NO_STONE) == LOW && Robot::instance()->stoneNumber<5){ //there is a stone
    if(Robot::instance()->direction){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      return;
    }
    else{
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      return;
    } 
  }
  else{ //no stone or all stones deposited already
    return; //dont move arm to centre
  }
}

void ManageStone::dropInStorage(){
  switch(Robot::instance()->stoneNumber){
    case 2: // incoming 6 inches // stone 1
      if(my_TEAM){
        Robot::instance()->armServo.write(160); //lower right
      }
      else{
        Robot::instance()->armServo.write(20); //lower left
      }
      break;
    case 3: // incoming 6 inches // stone 2
      pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(THREE_INCHES); // outgoing 9 inches 
      if(my_TEAM){
        Robot::instance()->armServo.write(140); //middle right
      }
      else{
        Robot::instance()->armServo.write(40); //middle left
      }
      break;
    case 4: // incoming 9 inches // stone 3
      pwm_start(CLAW_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(THREE_INCHES); // outgoing 12 inches
      Robot::instance()->armServo.write(60); //upper left
      break;
    case 5: // 12 inches //stone 4
      if(my_TEAM){
        Robot::instance()->armServo.write(40); //middle left
      }
      else{
        Robot::instance()->armServo.write(140); //middle right 
      }
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

