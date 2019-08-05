
#ifndef ManageStone_h
#define ManageStone_h

#include <Robot.h>

class ManageStone
{
  public:
    ManageStone(Robot const* robot);
    void collectStone(); //left = 1, right = 0
    void dropInStorage(); 

  private: 
    bool my_TEAM;
    // PinName motor;
    // void moveArmToPillar();
    //void moveArmToCentre();
    void turnClaw();
    //void raiseClaw();
    // int readSonar();
};

#endif