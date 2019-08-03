#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(Robot const* robot):
  my_PILLAR_DISTANCE(Robot::instance()->PILLAR_DISTANCE), 
  my_TEAM(Robot::instance()->TEAM)
{}

//stone 2, 3, 4,  same direction 
//stone 1 diff direction
//only translate all the way after picking up 1st stone, 
//no translating after that, only moving up from 6->9 after stone 2, 9->12 after stone 3
//dropInStorage right after picking up the stone except for 1st stone 

void ManageStone::collectStone(){ // dropInStorage moves claw to correct height + 2cm to not hit the stone.
  Robot::instance()->stoneNumber++;
  Robot::instance()->clawServo.write(0); // opening claw 
  if(my_TEAM){ // thanos
    Robot::instance()->armServo.write(180); //moving it to correct side TODO: team
  }
  else{ // methanos
    Robot::instance()->armServo.write(0);
  }
  // pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); //TODO: team
  // while(true){
  //   if(digitalRead(ARM_SIDES_LIMIT)==HIGH){
  //     pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
  //     break;
  //   }
  // }
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  Robot::instance()->clawServo.write(180); //deploying claw
  delay(900); // waiting for it to grasp
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(3500);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  //dropInStorage();
  Robot::instance()->state = GO_HOME;
  return;
}

void ManageStone::dropInStorage(){
  switch(Robot::instance()->stoneNumber){
    case 1: 
      Robot::instance()->armServo.write(90); //middle left or right 
      //check if middle can be accessed by either side 
  }
  Robot::instance()->clawServo.write(0); //open claw
  if(my_TEAM){ //thanos
    Robot::instance()->armServo.write(0); //default side opposite of whats expected due to our strategy
  }
  else{ //methanos
    Robot::instance()->armServo.write(180);
  }
  return;
}

void ManageStone::moveArmToPillar(){
  if(Robot::instance()->direction){
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  else{
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  while (true){
    if(readSonar()<=my_PILLAR_DISTANCE || digitalRead(ARM_SIDES_LIMIT) == HIGH){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
      return; 
    }
  }
}

void ManageStone::turnClaw(){
  if(Robot::instance()->direction){
    Robot::instance()->L_GauntletServo.write(180);
    Robot::instance()->R_GauntletServo.write(180);
    Robot::instance()->armServo.write(180); //180 degrees corresponds to right side
    Robot::instance()->L_GauntletServo.write(0);
    Robot::instance()->R_GauntletServo.write(0);
  }
  else{
    Robot::instance()->L_GauntletServo.write(180);
    Robot::instance()->R_GauntletServo.write(180);
    Robot::instance()->armServo.write(0);
    Robot::instance()->L_GauntletServo.write(0);
    Robot::instance()->R_GauntletServo.write(0);
  }
}

// void ManageStone::raiseClaw(){ //backup function using sonar 
//   if(Robot::instance()->collisionNumber != 0){
//     pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//     while(true){
//       if(readSonar()>=my_PILLAR_DISTANCE+3){ //check that sonar doesnt become negative
//         pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
//         return;
//       }
//     }
//   }
// }

int ManageStone::readSonar(){ //cm
  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);
  return pulseIn(SONAR_ECHO, HIGH)*(0.034/2);
}



