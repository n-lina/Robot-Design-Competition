#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
  state(FOLLOW_TAPE), 
  TEAM(true), 
  stoneNumber(0), 
  collisionNumber(0), 
  splitNumber(0), 
  direction_facing(true), 
  direction(true), 
  armServo(), 
  clawServo(), 
  L_GauntletServo(), 
  R_GauntletServo()
{
}

Robot* Robot::instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

void Robot::setup(){
    pinMode(ARM_TOP_BOTTOM_LIMIT, INPUT);
    pinMode(ARM_SIDES_LIMIT, INPUT);
    pinMode(SONAR_ECHO, INPUT); 
    pinMode(L_DECIDE, INPUT_PULLUP);
    pinMode(L_ALIGN, INPUT_PULLUP);
    pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
    pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
    pinMode(R_DECIDE, INPUT_PULLUP);
    pinMode(R_ALIGN, INPUT_PULLUP);
    pinMode(T_OR_M, INPUT);
    pinMode(COLLISION, INPUT_PULLUP);
  /////////////////////////////////////
    pinMode(SONAR_TRIG, OUTPUT);
    pinMode(ARM_MOTOR_LEFT, OUTPUT);
    pinMode(ARM_MOTOR_RIGHT, OUTPUT);
    pinMode(ARM_MOTOR_UP, OUTPUT);
    pinMode(ARM_MOTOR_DOWN, OUTPUT);
    pinMode(ARM_SERVO, OUTPUT);
    // pinMode(CLAW_SERVO, OUTPUT);
    pinMode(GAUNTLET_SERVO, OUTPUT);
    pinMode(LEFT_FORWARD_WHEEL_MOTOR, OUTPUT);
    pinMode(RIGHT_FORWARD_WHEEL_MOTOR, OUTPUT);
    pinMode(LEFT_BACKWARD_WHEEL_MOTOR, OUTPUT);
    pinMode(RIGHT_BACKWARD_WHEEL_MOTOR, OUTPUT);
    // pwm_start init motors
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(LEFT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(RIGHT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1);
    pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1);

  //   // Attaching servos 
    armServo.attach(ARM_SERVO);
    //clawServo.attach(CLAW_SERVO);
    L_GauntletServo.attach(GAUNTLET_SERVO);
    R_GauntletServo.attach(GAUNTLET_SERVO);
  //   // declaring interrupts

    // Creating stack for splits/tabs and Junction objects
    // Junction gauntletSplit(NOT_AVAIL, NOT_AVAIL, GAUNTLET_SPLIT);
    // Junction pathSplit(NOT_AVAIL, NOT_AVAIL, PATH_SPLIT);
    // Junction twelveInch_M(LARGE, 12, PILLAR_ONE);
    // Junction nineInch_M(LARGE, 9, PILLAR_TWO);
    // Junction sixInch_M(LARGE, 6, PILLAR_THREE);
    // Junction sixInch_T(SMALL, 6, PILLAR_FOUR);
    // Junction nineInch_T(LARGE, 9, PILLAR_FIVE);
    // Junction twelveInch_T(LARGE, 12, PILLAR_SIX);

    //Junction map 

    // Team 
  if(digitalRead(T_OR_M)==HIGH){
    TEAM = THANOS; //thanos
    direction = LEFT;
  }
  else{
    TEAM = METHANOS; //methanos
    direction = RIGHT;
  }
}

Junction::Junction(bool distanceToPillar, int setHeight, int junctionNumber):
  distance(distanceToPillar),
  height(setHeight), 
  number(junctionNumber)
{}





