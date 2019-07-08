
#ifndef ManageStone_h
#define ManageStone_h

//lower drawbridge in goHome function in FollowTape.h
#include "Servo.h"

class ManageStone
{
  public:
    ManageStone();
    void collectStone(); //left = 1, right = 0
    void dropInStorage(); 

  private:
    int stoneNumber; 
    Servo clawServo; 
    Servo armServo;
    PinName motor;
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