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
    TapeFollower();
    void turnInPlaceLeft();
    void turnInPlaceRight();
    void splitDecide();
    void followTape(); //polls 2 main PTs
    void goDistance(int distance); // follows tape without checking for splits or tabs 
    void goHome(); //after split #x, drop in gauntlet , polls corner PT 
    Servo L_GauntletServo;
    Servo R_GauntletServo;

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
    int L_lastEncoder;
    int L_encoder;
    int L_distance;
    int R_lastEncoder; 
    int R_encoder; 
    int R_distance;
    int getPosition(); //enum constant in order to know how to go home 
    void turnLeft();
    void turnRight();
    void stop();
};

#endif


