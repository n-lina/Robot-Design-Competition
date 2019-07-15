//#include <FollowTape.h>
  // go left 
  // go right 
  // stop?
  
#ifndef TapeFollower_h
#define TapeFollower_h

#include <Servo.h>

class TapeFollower
{
  public: 
    TapeFollower(int& state, int& stoneNumber, int& splitNumber, bool& TEAM, bool& direction_facing, bool& direction);
    void turnInPlaceLeft();
    void turnInPlaceRight();
    void splitDecide();
    void followTape(); //polls 2 main PTs
    void goDistance(int distance, bool firstRun, int checkptA, int checkptB); // follows tape without checking for splits or tabs 
    void goHome(); //after split #x, drop in gauntlet , polls corner PT 
    Servo L_GauntletServo;
    Servo R_GauntletServo;

  private:
    int& _state; 
    int& _stoneNumber; 
    int& _splitNumber; 
    bool& _TEAM; 
    bool& _direction_facing;
    bool& _direction;
    int collisionNumber;
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
    int lastEncoder;
    int encoder;
    int distance;
    void turnLeft();
    void turnRight();
    void stop();
};

#endif


