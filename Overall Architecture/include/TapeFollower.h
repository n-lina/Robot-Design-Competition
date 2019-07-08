//#include <FollowTape.h>
  // go left 
  // go right 
  // stop?
  
#ifndef TapeFollower_h
#define TapeFollower_h

class TapeFollower
{
  public: 
    TapeFollower();
    void turnInPlace();
    void splitDecide();
    void followTape(); //polls 2 main PTs
    void goHome(); //after split #x, drop in gauntlet , polls corner PT 

  private:
    int splitNumber;
    int derivative;  
    int loopCounter;
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
    int getPosition(); //enum constant in order to know how to go home 
    void turnLeft();
    void turnRight();
    void stop();
};

#endif


