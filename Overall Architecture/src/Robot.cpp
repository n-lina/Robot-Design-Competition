#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
<<<<<<< HEAD
  state(GO_DISTANCE), 
  TEAM(), 
  stoneNumber(0), 
  collisionNumber(0), 
  junctionNumber(0), 
  direction_facing(true), 
  direction(), 
  armServo(), 
  clawServo(), 
  L_GauntletServo(), 
  R_GauntletServo()
=======
state(FOLLOW_TAPE), TEAM(true), stoneNumber(0), collisionNumber(0), splitNumber(0), 
direction_facing(true), direction(true), 
armServo(), clawServo(), L_GauntletServo(), R_GauntletServo(), 
CV_Addresses{(int*) 0x0801FFF3, (int*) 0x0801FFF7, (int*) 0x0801FFFB, (int*) 0x0801FFEF, (int*) 0x0801FFDB, 
(int*)0x0801FFDF, (int*) 0x0801FFD7},
CV_Values{162, 12, 200, 200, 200, 4, YES_CALIBRATED},
labels{"KP Wheel", "KD Wheel", "On-Tape Threshold", "Decide Threshold", "Align Threshold",
"Pillar Distance (Cm)"},
value(0), lastEncoderValue(0), encoderValue(0),
display(Adafruit_SSD1306(-1))
>>>>>>> master
{
}

Robot* Robot::instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

<<<<<<< HEAD
void Robot::setup(){
  pinMode(ARM_TOP_BOTTOM_LIMIT, INPUT);
  pinMode(ARM_SIDES_LIMIT, INPUT);
=======
void Robot::setup(){    
  
  // Setting up pins
  pinMode(SONAR_ECHO, INPUT); 
>>>>>>> master
  pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP);
<<<<<<< HEAD
  pinMode(T_OR_M, INPUT);
  pinMode(COLLISION, INPUT_PULLUP);
  /////////////////////////////////////
  pinMode(ARM_MOTOR_H, OUTPUT);
  pinMode(ARM_MOTOR_V, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(GAUNTLET_SERVO, OUTPUT);
  pinMode(RIGHT_WHEEL, OUTPUT);
  pinMode(LEFT_WHEEL, OUTPUT);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(DEMULTIPLEX_ARM_H, OUTPUT);
  pinMode(DEMULTIPLEX_ARM_V, OUTPUT);
  pinMode(DEMULTIPLEX_R_WHEEL, OUTPUT);
  pinMode(DEMULTIPLEX_L_WHEEL, OUTPUT);
  // // pwm_start init motors
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_V, CLOCK_FQ, MAX_SPEED, 0, 1);

  // //   // Attaching servos 
=======
  pinMode(T_OR_M, INPUT_PULLUP);
  pinMode(CALIBRATE, INPUT_PULLUP);
  pinMode(COLLISION, INPUT_PULLUP);
  pinMode(TUNING_KNOB_A, INPUT_PULLUP);
  pinMode(TUNING_KNOB_B, INPUT_PULLUP);
  pinMode(TUNING_BUTTON, INPUT_PULLUP);
  pinMode(ARM_SIDES_LIMIT, INPUT_PULLUP);
  pinMode(ARM_TOP_BOTTOM_LIMIT, INPUT_PULLUP);
/////////////////////////////////////
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(ARM_MOTOR_LEFT, OUTPUT);
  pinMode(ARM_MOTOR_RIGHT, OUTPUT);
  pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
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

  // Attaching servos 
>>>>>>> master
  armServo.attach(ARM_SERVO);
  clawServo.attach(CLAW_SERVO);
  L_GauntletServo.attach(GAUNTLET_SERVO);
  R_GauntletServo.attach(GAUNTLET_SERVO);

  //   // Team 
  if(digitalRead(T_OR_M)==HIGH){
    TEAM = THANOS; 
  }
  else{
    TEAM = METHANOS; 
  }
}







