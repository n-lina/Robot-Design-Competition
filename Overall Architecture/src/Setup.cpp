#include "Arduino.h"
#include "Constants.h"
#include "Setup.h"
#include "ManageStone.h"
#include "TapeFollower.h"

TuningMenu::TuningMenu():
    pointers {p_KP_WHEEL,p_KD_WHEEL,p_THRESHOLD,p_SPLIT_THRESHOLD,p_GAUNTLET_TAPE_THRESHOLD,
    p_EDGE_THRESHOLD,p_COLLISION_THRESHOLD},
    lastEncoderValue(0),
    encoderValue(0), value(0),
    labels {"KP Tape Following","KD Tape Following","Line Following Threshold",
    "Split Detected Threshold","Gauntlet Tape Threshold","Edge Threshold", "Collision Threshold"},
    display(Adafruit_SSD1306(-1)) 
{
}

void TuningMenu::toggleMenu(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    
    for(volatile int i=0; i<7; i++){
      display.clearDisplay();
      display.setCursor(0,0);
      display.print(labels[i]);
      display.display();
      display.setCursor(0,15);

      value = *pointers[i];

      while(digitalRead(TUNING_BUTTON)==LOW){ //while button not pressed 
        encoderValue = digitalRead(TUNING_KNOB_A);
        if(encoderValue != lastEncoderValue){
          if(digitalRead(TUNING_KNOB_B) != encoderValue){
            value ++;
          }
          else{
            value --;
          }
        display.print(value);
        display.display();
        lastEncoderValue = encoderValue;
      }
    }
    *pointers[i] = value;         
  }
  return;
}

void TuningMenu::adjustVariables(){
  if(*p_CALIBRATED != YES_CALIBRATED){
    *p_KP_WHEEL = 30;
    *p_KD_WHEEL = 9;
    *p_THRESHOLD = 200;
    *p_SPLIT_THRESHOLD = 200;
    *p_GAUNTLET_TAPE_THRESHOLD = 200;
    *p_EDGE_THRESHOLD = 15;
    *p_COLLISION_THRESHOLD = 5;
    *p_CALIBRATED = YES_CALIBRATED;
    return;
  }
  if(digitalRead(CALIBRATE)==HIGH){
    toggleMenu();
    return;
  }
}

Setup::Setup(){}

static void Setup::setup(){
    
  // Methanos or Thanos
  switch (digitalRead(T_OR_M)){
    case HIGH: 
      TEAM = true;
    case LOW:
      TEAM = false;
  }

  // Instantiating Classes
  ManageStone stoneRobot; 
  TapeFollower tapeRobot; 
  TuningMenu tuneRobot; 

  Servo servos [4] = {stoneRobot.armServo, stoneRobot.clawServo, tapeRobot.L_GauntletServo, 
  tapeRobot.R_GauntletServo};
  
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
  for (volatile int j=12; j<18; j++){
      pinMode(inputPins[j], INPUT_PULLUP);
  }
}

// put RC filter for switch debouncing 
