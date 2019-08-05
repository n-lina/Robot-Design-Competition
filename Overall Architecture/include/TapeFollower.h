//#include <FollowTape.h>
  // go left 
  // go right 
  // stop?

//TODO:
  //interrupts for collision, edge, go home, how to have global,unchanging-once-declared variables, 
  //static functions and variables for Setup library 
  
#ifndef TapeFollower_h
#define TapeFollower_h

#include <Robot.h>

class TapeFollower
{
  public: 
    TapeFollower(Robot const* robot);
    void splitDecide(); //TODO
    void followTape(); //polls 2 main PTs
    void goDistance(int loopNumber); 
    // follows tape without checking for splits or tabs 
    void goHome(); //only call from between first tabs and 2nd split
    void park();
    void avoidCollision();

  private:
    bool my_TEAM;
    int derivative;  
    int default_speed;
    int timeStep; 
    int position; 
    int lastPosition; 
    int PID; 
    int number; // used for derivative
    int debounce; // so it does not over-read junctions
    bool leftTapeFollow;
    bool rightTapeFollow;
    bool leftDecide;
    bool rightDecide;
    bool leftAlign;
    bool rightAlign;
    int lastEncoder;
    int encoder;
    int loopCounter;
    bool pressed;
    bool homeSplit;
    void stop();
    void turnLeft();
    void turnRight();
    //void goStraight();
    //void turnLeftSoft();
    //void turnRightSoft();
    void turnInPlaceLeft();
    void turnInPlaceRight();
    void alignPillar();
    void dropGauntlet();
};

#endif


