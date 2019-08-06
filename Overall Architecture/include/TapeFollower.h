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
#include <stack>
#include "Constants.h"

class TapeFollower
{
  public: 
    TapeFollower(Robot const* robot);
    void splitDecide(); 
    void followTape(); 
    void goDistance(int loopNumber); 
    // follows tape without checking for splits or tabs 
    void goHome(); 
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
    void stop();
    void turnLeft();
    void turnRight();
    void turnInPlaceLeft();
    void turnInPlaceRight();
    void turnRightAfterPillar();
    void alignPillarSixTHANOS();
    void getPosition();
    void dropGauntlet();
    void alignPillar();
    std::stack<bool> my_path;
   // std::vector<Junction> topPath;
   // std::vector<Junction> bottomPath;
    bool topPath [TOP_PATH_SIZE];
    bool bottomPath [BOTTOM_PATH_SIZE];
};

#endif


