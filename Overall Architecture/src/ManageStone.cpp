#include <Arduino.h>
#include "ManageStone.h"
#include "Constants.h"

ManageStone::ManageStone(Robot const* robot):
my_TEAM(Robot::instance()->TEAM)
{}

void ManageStone::collectStone(){ // dropInStorage moves claw to correct height + 2cm to not hit the stone.
  Robot::instance()->clawServo.write(0); // opening claw 
  if(Robot::instance()->direction == LEFT){ 
    Robot::instance()->armServo.write(180); 
  }
  else{ 
    Robot::instance()->armServo.write(0);
  }
  Robot::instance()->stoneNumber++;
  switch(Robot::instance()->stoneNumber){
    case 1: 
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ,  MAX_SPEED, MAX_SPEED, 0);
      delay(4000);
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
      Robot::instance()->clawServo.write(180);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(4000);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
      delay(100);
      // pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
      // pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
      // delay(500);
      pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 600, 0);
      pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
      delay(500);
      if(METHANOS) Robot::instance()->state = TURN_IN_PLACE_RIGHT;
      else Robot::instance()->state = TURN_IN_PLACE_LEFT;
      return;
    case 2: 
      dropInStorage(1); // stone 1
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ,  MAX_SPEED, MAX_SPEED, 0);
      delay(4000);
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
      Robot::instance()->clawServo.write(180);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); //picking stone up 
      delay(4000);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
      dropInStorage(2); // stone 2
      pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
      delay(500);
      if(METHANOS) Robot::instance()->state = TURN_IN_PLACE_RIGHT;
      else Robot::instance()->state = TURN_IN_PLACE_LEFT;
      return;
  }
  return;
}

void ManageStone::dropInStorage(int stoneCollected){
  if(Robot::instance()->direction == LEFT){
    switch(stoneCollected){
      case 1: 
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        Robot::instance()->L_GauntletServo.write(100);
        Robot::instance()->armServo.write(94);
        delay(500);
        Robot::instance()->clawServo.write(0);
        delay(500);
        for(volatile int i=100; i>=70; i=i-3){
          Robot::instance()->L_GauntletServo.write(i);
          delay(200);
        }
        delay(300);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        break;
      case 2: 
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        Robot::instance()->L_GauntletServo.write(100);
        Robot::instance()->armServo.write(0);
        delay(500);
        for(volatile int i=100; i>=70; i=i-3){
          Robot::instance()->L_GauntletServo.write(i);
          delay(200);
        }
        delay(300);    
        break;  
    }
  }
  else{ //direction == RIGHT 
    switch(stoneCollected){
      case 1: 
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        Robot::instance()->L_GauntletServo.write(100);
        Robot::instance()->armServo.write(94);
        delay(500);
        Robot::instance()->clawServo.write(0);
        delay(500);
        for(volatile int i=100; i>=70; i=i-3){
          Robot::instance()->L_GauntletServo.write(i);
          delay(200);
        }
        delay(300);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        break;
      case 2: 
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(4000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
        delay(100);
        Robot::instance()->L_GauntletServo.write(100);
        Robot::instance()->armServo.write(0);
        delay(500);
        for(volatile int i=100; i>=70; i=i-3){
          Robot::instance()->L_GauntletServo.write(i);
          delay(200);
        }
        delay(300);    
        break;  
    }
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



