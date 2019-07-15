
#ifndef ManageStone_h
#define ManageStone_h

//lower drawbridge in goHome function in FollowTape.h
#include "Servo.h"

class ManageStone
{
  public:
    ManageStone(int& stoneNumber, int& state, bool& direction);
    void collectStone(); //left = 1, right = 0
    void dropInStorage(); 
    Servo clawServo; 
    Servo armServo;

  private: 
    int& _stoneNumber; 
    int& _state;
    bool* _direction;
    bool lastEncoderState;
    bool encoderState;
    int clawHeight; 
    PinName motor;
    void moveArmToPillar();
    void moveArmToCentre();
    void turnClaw();
    void raiseClaw();
    void lowerClaw();
};

#endif