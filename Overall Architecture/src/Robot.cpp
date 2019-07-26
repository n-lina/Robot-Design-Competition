#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
state(FOLLOW_TAPE), TEAM(true), stoneNumber(0), collisionNumber(0), splitNumber(0),  direction_facing(true), direction(true),
KP_WHEEL(KP_WHEEL_), KD_WHEEL(KD_WHEEL_), THRESHOLD(THRESHOLD_), SPLIT_THRESHOLD(THRESHOLD_), 
TAB_THRESHOLD(TAB_THRESHOLD_), PILLAR_DISTANCE(PILLAR_DISTANCE_), 
armServo(), clawServo(), L_GauntletServo(), R_GauntletServo(), 
CV_Addresses{(int*) 0x0801FFF3, (int*) 0x0801FFF7, (int*) 0x0801FFFB, (int*) 0x0801FFEF, (int*) 0x0801FFDB, 
(int*)0x0801FFDF, (int*) 0x0801FFD7},
CV_Values{162, 12, 200, 200, 200, 4, YES_CALIBRATED},
labels{"KP Wheel", "KD Wheel", "On-Tape Threshold", "Split Threshold", "Tab Threshold",
"Pillar Distance (Cm)"},
value(0), lastEncoderValue(0), encoderValue(0),
display(Adafruit_SSD1306(-1))
{
}

Robot* Robot::instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

void Robot::setup(){    
  
  // Setting up pins
  pinMode(L_WHEEL_ENCODER, INPUT_PULLUP); //check if input or input pullup
  pinMode(R_WHEEL_ENCODER, INPUT_PULLUP);
  pinMode(ARM_SONAR_ECHO, INPUT); 
  pinMode(TUNING_KNOB, INPUT_PULLUP);
  pinMode(L_TAB, INPUT_PULLUP);
  pinMode(L_SPLIT, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_SPLIT, INPUT_PULLUP);
  pinMode(R_TAB, INPUT_PULLUP);
  pinMode(CALIBRATE, INPUT_PULLUP);
  pinMode(T_OR_M, INPUT_PULLUP);
  pinMode(MULTIPLEX_OUT, INPUT_PULLUP);
  pinMode(COLLISION, INPUT_PULLUP);
/////////////////////////////////////
  pinMode(ARM_SONAR_TRIGGER, OUTPUT);
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
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(MULTIPLEX_C, OUTPUT);

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
  armServo.attach(ARM_SERVO);
  clawServo.attach(CLAW_SERVO);
  L_GauntletServo.attach(GAUNTLET_SERVO);
  R_GauntletServo.attach(GAUNTLET_SERVO);

  // declaring interrupts

  // team 
  switch (digitalRead(T_OR_M)){
    case HIGH: 
      TEAM = true; //thanos
      direction = LEFT;
    case LOW:
      TEAM = false; //methanos
      direction = RIGHT;
  }
  
  //adjustVariables();
}

void Robot::toggleMenu(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    
    for(volatile int i=0; i<NUM_VARIABLES-1; i++){
      display.clearDisplay();
      display.setCursor(0,0);
      display.print(labels[i]);
      display.display();
      display.setCursor(0,15);

      value = *CV_Addresses[i];

      while(!multi(1,0,1)){ //while tuning button not pressed 
      //TODO
        encoderValue = digitalRead(TUNING_KNOB);
        if(encoderValue != lastEncoderValue){
          if(digitalRead(TUNING_KNOB) != encoderValue){
            value ++;
          }
          else{
            value --;
          }
      ////
        display.print(String(value));
        display.display();
        lastEncoderValue = encoderValue;
      }
    }
    *CV_Addresses[i] = value;         
  }
  return;
}


void Robot::adjustVariables(){
  if(*CV_Addresses[CALIBRATED_MAGIC] != YES_CALIBRATED){
    for(volatile int i=0; i<NUM_VARIABLES; i++){
      *CV_Addresses[i] = CV_Values[i];
    }
  }
  if(digitalRead(CALIBRATE)==HIGH){
    toggleMenu();
  }
  KP_WHEEL = *CV_Addresses[KP_WHEEL];
  KD_WHEEL = *CV_Addresses[KD_WHEEL];
  THRESHOLD = *CV_Addresses[THRESHOLD];
  SPLIT_THRESHOLD = *CV_Addresses[SPLIT_THRESHOLD];
  TAB_THRESHOLD = *CV_Addresses[TAB_THRESHOLD];
  PILLAR_DISTANCE =*CV_Addresses[PILLAR_DISTANCE];
  return;
}

bool Robot::multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C); 
  return digitalRead(MULTIPLEX_OUT);
}


