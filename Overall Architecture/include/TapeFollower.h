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
    void turnInPlaceLeft();
    void turnInPlaceRight();
    void splitDecide();
    void followTape(); //polls 2 main PTs
    void goDistance(int distance, bool firstRun, int checkptA, int checkptB); // follows tape without checking for splits or tabs 
    void goHome(bool park); //TODO

  private:
    int my_KP_WHEEL;
    int my_KD_WHEEL;
    int my_THRESHOLD; 
    int my_SPLIT_THRESHOLD;
    int my_TAB_THRESHOLD;
    int my_ALIGN_TAB_THRESHOLD; 
    int my_EDGE_THRESHOLD;
    int my_COLLISION_THRESHOLD;
    int derivative;  
    int default_speed;
    int timeStep; 
    int position; 
    int lastPosition; 
    int PID; 
    int number;
    bool leftSensor;
    bool rightSensor;
    bool leftSplit;
    bool rightSplit;
    bool leftTab;
    bool rightTab;
    int lastEncoder;
    int encoder;
    int distance;
    void turnLeft();
    void turnRight();
    void stop();
    void goStraight();
    void alignLeftTab(); //TODO
    void alignRightTab(); //TODO
};

#endif


