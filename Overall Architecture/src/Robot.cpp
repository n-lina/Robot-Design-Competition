#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
state(0), TEAM(true), stoneNumber(0), collisionNumber(0), splitNumber(0),  direction_facing(true), direction(true),
armServo(), clawServo(), L_GauntletServo(), R_GauntletServo(), 
CV_Addresses{(int*) 0x0801FFF3, (int*) 0x0801FFF7, (int*) 0x0801FFFB, (int*) 0x0801FFEF, (int*) 0x0801FFDB, 
(int*) 0x0801FFEB, (int*) 0x0801FFE7, (int*) 0x0801FFE3, (int*)0x0801FFDF, (int*) 0x0801FFD7},
CV_Values{30, 9, 200, 200, 200, 200, 15, 5, 3, YES_CALIBRATED},
labels{"KP Wheel", "KD Wheel", "On-Tape Threshold", "Split Threshold", "Tab Threshold", "Alignment Tab Threshold", 
"Edge Threshold", "Collision Threshold", "Pillar Distance"},
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
  switch (digitalRead(T_OR_M)){
    case HIGH: 
      TEAM = true; //thanos

    case LOW:
      TEAM = false; //methanos
  }
    
  PinName outputPins [12] ={ARM_SERVO, ARM_MOTOR_LEFT, ARM_MOTOR_RIGHT, CLAW_SERVO, CLAW_MOTOR_UP, CLAW_MOTOR_DOWN, 
  L_GAUNTLET_SERVO, LEFT_FORWARD_WHEEL_MOTOR, RIGHT_FORWARD_WHEEL_MOTOR, R_GAUNTLET_SERVO, LEFT_BACKWARD_WHEEL_MOTOR, 
  RIGHT_BACKWARD_WHEEL_MOTOR};
  // input_pullup or output

  PinName inputPins [17] = {ARM_SONAR, CLAW_ENCODER, ARM_HOME_SWITCH, TUNING_KNOB_A, TUNING_KNOB_B, TUNING_BUTTON,
  L_TAPE_FOLLOW, R_TAPE_FOLLOW, L_SPLIT, R_SPLIT, L_TAB, R_TAB, L_TAB_ALIGN, MIDDLE_TAB_ALIGN, R_TAB_ALIGN, CALIBRATE, T_OR_M};
  
  Servo servos [4] = {armServo, clawServo, L_GauntletServo, R_GauntletServo};
  
  // Setting up pins
  for (volatile int i=0; i<12; i++){
    pinMode(outputPins[i], OUTPUT);
    pinMode(inputPins[i], INPUT_PULLUP);
    if((i%3)==0){
      servos[i/3].attach(outputPins[i]);
    }
    else{
      pwm_start(outputPins[i], CLOCK_FQ, MAX_SPEED, 0, 1);      
    }
  }
  for (volatile int j=12; j<17; j++){
    pinMode(inputPins[j], INPUT_PULLUP);
  }

  adjustVariables();
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

      while(digitalRead(TUNING_BUTTON)==LOW){ //while button not pressed 
        encoderValue = digitalRead(TUNING_KNOB_A);
        if(encoderValue != lastEncoderValue){
          if(digitalRead(TUNING_KNOB_B) != encoderValue){
            value ++;
          }
          else{
            value --;
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
  if(digitalRead(CALIBRATE)==HIGH){
    toggleMenu();
  }
  KP_WHEEL = *CV_Addresses[KP_WHEEL];
  KD_WHEEL = *CV_Addresses[KD_WHEEL];
  THRESHOLD = *CV_Addresses[THRESHOLD];
  SPLIT_THRESHOLD = *CV_Addresses[SPLIT_THRESHOLD];
  TAB_THRESHOLD = *CV_Addresses[TAB_THRESHOLD];
  ALIGN_TAB_THRESHOLD = *CV_Addresses[ALIGN_TAB_THRESHOLD];
  EDGE_THRESHOLD = *CV_Addresses[EDGE_THRESHOLD];
  COLLISION_THRESHOLD = *CV_Addresses[COLLISION_THRESHOLD];
  PILLAR_DISTANCE =*CV_Addresses[PILLAR_DISTANCE];
  return;
}


