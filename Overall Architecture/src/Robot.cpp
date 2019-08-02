#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
state(FOLLOW_TAPE), TEAM(true), stoneNumber(0), collisionNumber(0), splitNumber(0), 
direction_facing(true), direction(true), 
armServo(), clawServo(), L_GauntletServo(), R_GauntletServo(), 
CV_Addresses{(int*) 0x0801FFF3, (int*) 0x0801FFF7, (int*) 0x0801FFFB, (int*) 0x0801FFEF, (int*) 0x0801FFDB, 
(int*)0x0801FFDF, (int*) 0x0801FFD7},
CV_Values{162, 12, 200, 200, 200, 4, YES_CALIBRATED},
labels{"KP Wheel", "KD Wheel", "On-Tape Threshold", "Decide Threshold", "Align Threshold",
"Pillar Distance (Cm)"},
value(0), lastEncoderValue(0), encoderValue(0), increment(1),
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
//   pinMode(L_ENCODER_A, INPUT_PULLUP); //check if input or input pullup
//   pinMode(L_ENCODER_B, INPUT_PULLUP);
//   pinMode(R_ENCODER_A, INPUT_PULLUP);
//   pinMode(R_ENCODER_B, INPUT_PULLUP);
//   pinMode(SONAR_ECHO, INPUT); 
//   pinMode(TUNING_KNOB, INPUT_PULLUP);
//   pinMode(L_DECIDE, INPUT_PULLUP);
//   pinMode(L_ALIGN, INPUT_PULLUP);
//   pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
//   pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
//   pinMode(R_DECIDE, INPUT_PULLUP);
//   pinMode(R_ALIGN, INPUT_PULLUP);
//   pinMode(T_OR_M, INPUT_PULLUP);
//   pinMode(MULTIPLEX_OUT, INPUT_PULLUP);
//   pinMode(COLLISION, INPUT_PULLUP);
// /////////////////////////////////////
//   pinMode(SONAR_TRIG, OUTPUT);
//   pinMode(ARM_MOTOR_LEFT, OUTPUT);
//   pinMode(ARM_MOTOR_RIGHT, OUTPUT);
//   pinMode(ARM_MOTOR_UP, OUTPUT);
//   pinMode(ARM_MOTOR_DOWN, OUTPUT);
//   pinMode(ARM_SERVO, OUTPUT);
//   pinMode(CLAW_SERVO, OUTPUT);
//   pinMode(GAUNTLET_SERVO, OUTPUT);
//   pinMode(LEFT_FORWARD_WHEEL_MOTOR, OUTPUT);
//   pinMode(RIGHT_FORWARD_WHEEL_MOTOR, OUTPUT);
//   pinMode(LEFT_BACKWARD_WHEEL_MOTOR, OUTPUT);
//   pinMode(RIGHT_BACKWARD_WHEEL_MOTOR, OUTPUT);
//   pinMode(MULTIPLEX_A, OUTPUT);
//   pinMode(MULTIPLEX_B, OUTPUT);
//   pinMode(MULTIPLEX_C, OUTPUT);

  // // pwm_start init motors
  // pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(LEFT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(RIGHT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1);
  // pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1);

  // // Attaching servos 
  // armServo.attach(ARM_SERVO);
  // clawServo.attach(CLAW_SERVO);
  // L_GauntletServo.attach(GAUNTLET_SERVO);
  // R_GauntletServo.attach(GAUNTLET_SERVO);

  // // declaring interrupts

  // // Creating stack for splits/tabs and Junction objects
  // Junction gauntletSplit(NOT_AVAIL, NOT_AVAIL, GAUNTLET_SPLIT);
  // Junction pathSplit(NOT_AVAIL, NOT_AVAIL, PATH_SPLIT);
  // Junction twelveInch_M(LARGE, 12, PILLAR_ONE);
  // Junction nineInch_M(LARGE, 9, PILLAR_TWO);
  // Junction sixInch_M(LARGE, 6, PILLAR_THREE);
  // Junction sixInch_T(SMALL, 6, PILLAR_FOUR);
  // Junction nineInch_T(LARGE, 9, PILLAR_FIVE);
  // Junction twelveInch_T(LARGE, 12, PILLAR_SIX);

  // //Junction map 

  // // Team 
  // switch (digitalRead(T_OR_M)){
  //   case HIGH: 
  //     TEAM = true; //thanos
  //     direction = LEFT;
  //     break;
  //   case LOW:
  //     TEAM = false; //methanos
  //     direction = RIGHT;
  //     break;
  // }

  // Calibrating / assigning values 
  pinMode(TUNING_BUTTON, INPUT_PULLUP);
  pinMode(TUNING_KNOB_A, INPUT_PULLUP);
  pinMode(TUNING_KNOB_B, INPUT_PULLUP);
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

    while(digitalRead(TUNING_BUTTON)==LOW){ // while button not pressed 
      encoderValue = digitalRead(TUNING_KNOB_A);
      if(encoderValue != lastEncoderValue){
        if(digitalRead(TUNING_KNOB_B) != encoderValue){
          value++; 
        }
        else{
          value--;
        }
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
  //if(digitalRead(CALIBRATE)==HIGH){
    toggleMenu();
  //}
  KP_WHEEL = *CV_Addresses[KP_WHEEL];
  KD_WHEEL = *CV_Addresses[KD_WHEEL];
  THRESHOLD = *CV_Addresses[THRESHOLD];
  DECIDE_THRESHOLD = *CV_Addresses[DECIDE_THRESHOLD];
  ALIGN_THRESHOLD = *CV_Addresses[ALIGN_THRESHOLD];
  PILLAR_DISTANCE =*CV_Addresses[PILLAR_DISTANCE];
  return;
}

bool Robot::multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C); 
  return digitalRead(MULTIPLEX_OUT);
}

Junction::Junction(bool distanceToPillar, int setHeight, int junctionNumber){
  distance = distanceToPillar;
  height = setHeight; 
  number = junctionNumber;
}


