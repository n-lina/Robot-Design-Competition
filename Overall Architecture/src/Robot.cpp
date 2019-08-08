#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
  state(GO_DISTANCE), 
  TEAM(), 
  stoneNumber(0), 
  collisionNumber(0), 
  junctionNumber(0), 
  direction_facing(true), 
  direction(), 
  armServo(), 
  clawServo(), 
  L_GauntletServo()
{
}

Robot* Robot::instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

void Robot::setup(){
  pinMode(ARM_SIDES_LIMIT, INPUT);
  pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP);
  pinMode(T_OR_M, INPUT);
  pinMode(COLLISION, INPUT_PULLUP);
  /////////////////////////////////////
  pinMode(ARM_MOTOR_RIGHT, OUTPUT);
  pinMode(ARM_MOTOR_LEFT, OUTPUT);
  pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(GAUNTLET_SERVO, OUTPUT);
  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_BACKWARD, OUTPUT);
  pinMode(LEFT_WHEEL_BACKWARD, OUTPUT);

  // // pwm_start init motors
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1);

   // Attaching servos 
  armServo.attach(ARM_SERVO);
  clawServo.attach(CLAW_SERVO);
  L_GauntletServo.attach(GAUNTLET_SERVO);

  clawServo.write(0);
  L_GauntletServo.write(100);

  if(digitalRead(T_OR_M) == HIGH){
    TEAM = METHANOS; 
    armServo.write(0);
    pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  else{
    TEAM = THANOS; 
    armServo.write(180);
    pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }

}










