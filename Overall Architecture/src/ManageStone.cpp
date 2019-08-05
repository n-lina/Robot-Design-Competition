#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(Robot const* robot):
my_TEAM(Robot::instance()->TEAM)
{}

void ManageStone::collectStone(){ // dropInStorage moves claw to correct height + 2cm to not hit the stone.
  Robot::instance()->stoneNumber++;
  Robot::instance()->clawServo.write(0); // opening claw 
  if(Robot::instance()->direction == LEFT){ 
    Robot::instance()->armServo.write(180); 
  }
  else{ 
    Robot::instance()->armServo.write(0);
  }
  Robot::instance()->clawServo.write(0);
  Robot::instance()->armServo.write(0);
  digitalWrite(DEMULTIPLEX_ARM_V, LOW);
  pwm_start(ARM_MOTOR_V, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(ARM_MOTOR_V, CLOCK_FQ, MAX_SPEED, 0, 0);
  Robot::instance()->clawServo.write(180); //deploying claw
  delay(850); // waiting for it to grasp
  digitalWrite(DEMULTIPLEX_ARM_V, HIGH);
  pwm_start(ARM_MOTOR_V, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(3500);
  // pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  // Robot::instance()->armServo.write(180);
  // delay(500);
  // clawServo.write(0);
  dropInStorage();
  // Robot::instance()->state = GO_HOME;
  Robot::instance()->state = FOLLOW_TAPE;
  return;
}

void ManageStone::dropInStorage(){
  switch(Robot::instance()->stoneNumber){
    case 1: 
      Robot::instance()->armServo.write(90); //middle left or right 
      delay(600);
      //check if middle can be accessed by either side 
  }
  Robot::instance()->clawServo.write(0); //open claw
  if(Robot::instance()->direction == LEFT){ 
    Robot::instance()->armServo.write(0); //default side facing inside of robot
  }
  else{ 
    Robot::instance()->armServo.write(180);
  }
  return;
}

// void ManageStone::moveArmToPillar(){
//   if(Robot::instance()->direction){
//       pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//   }
//   else{
//       pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//   }
//   while (true){
//     if(readSonar()<=my_PILLAR_DISTANCE || digitalRead(ARM_SIDES_LIMIT) == HIGH){
//       pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
//       pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);   
//       return; 
//     }
//   }
// }

// void ManageStone::turnClaw(){
//   if(Robot::instance()->direction){
//     Robot::instance()->L_GauntletServo.write(180);
//     Robot::instance()->R_GauntletServo.write(180);
//     Robot::instance()->armServo.write(180); 
//     Robot::instance()->L_GauntletServo.write(0);
//     Robot::instance()->R_GauntletServo.write(0);
//   }
//   else{
//     Robot::instance()->L_GauntletServo.write(180);
//     Robot::instance()->R_GauntletServo.write(180);
//     Robot::instance()->armServo.write(0);
//     Robot::instance()->L_GauntletServo.write(0);
//     Robot::instance()->R_GauntletServo.write(0);
//   }
// }

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

// int ManageStone::readSonar(){ //cm
//   digitalWrite(SONAR_TRIG, LOW);
//   delayMicroseconds(2);
//   digitalWrite(SONAR_TRIG, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(SONAR_TRIG, LOW);
//   return pulseIn(SONAR_ECHO, HIGH)*(0.034/2);
// }



