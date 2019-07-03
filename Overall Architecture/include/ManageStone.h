
#ifndef ManageStone_h
#define ManageStone_h

  // raise/lower claw 
  // close / open claw 
  // lower drawbridge
  // turn claw - .write() 

#include "Arduino.h"
#include "Servo.h"

class ManageStone
{
  public:
    ManageStone();
    void collectStone(); //left = 1, right = 0
    void dropInStorage(); 
    void dropInGauntlet();   
  private:
    bool& _direction; //bottom path or top path; methanos or thanos 
    int stoneNumber; 
    Servo clawServo; 
    Servo armServo;
    Servo gauntletServo;
    bool lastEncoderState;
    bool encoderState;
    int clawHeight; 
    void moveArmToPillar();
    void moveArmToCentre();
    void turnClaw();
    void raiseClaw();
    void lowerClaw();
};

#endif