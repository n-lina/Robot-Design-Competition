#include <TuningMenu.h>
#include <Constants.h>
#include <Arduino.h>

TuningMenu::TuningMenu():
    lastEncoderValue(0), encoderValue(0), value(0),
    labels {"KP Tape Following","KD Tape Following","Line Following Threshold",
    "Split Detected Threshold","Gauntlet Tape Threshold","Edge Threshold", "Collision Threshold"},
    display(Adafruit_SSD1306(-1)), p_KP_WHEEL((int*) 0x0801FFF3), p_KD_WHEEL((int*) 0x0801FFF7), 
    p_THRESHOLD((int*) 0x0801FFFB), p_SPLIT_THRESHOLD((int*) 0x0801FFEF), p_TAB_THRESHOLD((int*) 0x0801FFDB),
    p_GAUNTLET_TAPE_THRESHOLD((int*) 0x0801FFEB), p_EDGE_THRESHOLD((int*) 0x0801FFE7), 
    p_COLLISION_THRESHOLD((int*) 0x0801FFE3), p_PILLAR_DISTANCE((int*)0x0801FFDF), p_CALIBRATED((int*) 0x0801FFD7)
{
}

void TuningMenu::toggleMenu(){
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    
    volatile int* pointers [NUM_VARIABLES] = {p_KP_WHEEL,p_KD_WHEEL,p_THRESHOLD,p_SPLIT_THRESHOLD,p_TAB_THRESHOLD, p_GAUNTLET_TAPE_THRESHOLD,
    p_EDGE_THRESHOLD, p_COLLISION_THRESHOLD, p_PILLAR_DISTANCE, p_CALIBRATED};
    
    for(volatile int i=0; i<NUM_VARIABLES; i++){
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
    *p_PILLAR_DISTANCE = 3;
    *p_CALIBRATED = YES_CALIBRATED;
    return;
  }
  if(digitalRead(CALIBRATE)==HIGH){
    toggleMenu();
    return;
  }
}

void TuningMenu::loadVariables(){
  
}