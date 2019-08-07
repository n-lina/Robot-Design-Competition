#ifndef Robot_h
#define Robot_h

// #include <Wire.h>
// #include <Adafruit_SSD1306.h>
// #include <FreeMono12pt7b.h>
#include <Servo.h>
#include <stack>

using namespace std;

enum RobotStates {
   GO_DISTANCE,
   FOLLOW_TAPE,
   AVOID_COLLISION,
   SPLIT_CHOOSER,
   COLLECT_STONE, 
   GO_HOME,
   TURN_IN_PLACE_RIGHT,
   TURN_IN_PLACE_LEFT,
   PARK,
   NUM_STATES,
};

enum Splits {
   GAUNTLET_SPLIT = 1,
   PATH_SPLIT,
   NUM_SPLITS
};

enum Pillars {
   PILLAR_ONE = 1,
   PILLAR_TWO, 
   PILLAR_THREE, 
   PILLAR_FOUR,
   PILLAR_FIVE,
   PILLAR_SIX
};

class Robot {
public:
   static Robot* instance(); 
   const int val = 10;
   void setup();
   int state; 
   bool TEAM; //true = thanos, false = methanos 
   bool buttonPrevPressed;
   int stoneNumber; 
   int collisionNumber;
   int junctionNumber;
   bool direction_facing; //true = forwards, false = backwards
   bool direction; // true = right, left = false 
   Servo armServo; // 0 = left; 180 = right
   Servo clawServo; // 0 = open; 180 = closed
   Servo L_GauntletServo;
   Servo R_GauntletServo;
   int DECIDE_THRESHOLD; 
   int ALIGN_THRESHOLD; 
   int THRESHOLD; 
  // Adafruit_SSD1306 display;

private:
   Robot();  // Private so that it can not be called
   Robot(Robot const& bot){};  // copy constructor is private
   Robot& operator=(Robot const& bot){}; // assignment operator is private
   static Robot* m_pInstance;
 //  bool multi(bool A, bool B);
};


#endif