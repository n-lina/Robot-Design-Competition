
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
    int my_PILLAR_DISTANCE;
    bool my_TEAM;
    PinName motor;
    void moveArmToPillar();
    void moveArmToCentre();
    void turnClaw();
    void raiseClaw();
    int readSonar();
    bool multi(bool C, bool B, bool A);
};

#endif