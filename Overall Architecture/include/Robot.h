#ifndef Robot_h
#define Robot_h

#include <Adafruit_SSD1306.h>
#include <FreeMono12pt7b.h>
#include <Servo.h>

using namespace std;

enum RobotStates {
   GO_DISTANCE,
   FOLLOW_TAPE,
   AVOID_COLLISION,
   SPLIT_CHOOSER,
   COLLECT_STONE, 
   GO_HOME,
   PARK,
   NUM_STATES,
};

enum Calibration_Values {
  KP_WHEEL,
  KD_WHEEL,
  THRESHOLD,
  DECIDE_THRESHOLD,
  ALIGN_THRESHOLD,
  PILLAR_DISTANCE,
  CALIBRATED_MAGIC,
  NUM_VARIABLES
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
   void setup();
   int state; 
   bool TEAM; //true = thanos, false = methanos 
   int stoneNumber; 
   int collisionNumber;
   int splitNumber;
   bool direction_facing; //true = forwards, false = backwards
   bool direction; // true = right, left = false 
   // Junction firstRun [];
   // Junction secondRun [];
   int KP_WHEEL; 
   int KD_WHEEL; 
   int THRESHOLD; 
   int DECIDE_THRESHOLD; 
   int ALIGN_THRESHOLD; 
   int PILLAR_DISTANCE; 
   Servo armServo; // 0 = left; 180 = right
   Servo clawServo; // 0 = open; 180 = closed
   Servo L_GauntletServo;
   Servo R_GauntletServo;

private:
   Robot();  // Private so that it can  not be called
   Robot(Robot const& bot){};  // copy constructor is private
   Robot& operator=(Robot const& bot){}; // assignment operator is private
   static Robot* m_pInstance;
   void toggleMenu();
   void adjustVariables();
   bool multi(bool C, bool B, bool A);
   int* CV_Addresses [NUM_VARIABLES];
   int CV_Values [NUM_VARIABLES];
   String labels [NUM_VARIABLES-1];
   volatile int value; 
   bool lastEncoderValue;
   bool encoderValue;
   int increment;
   Adafruit_SSD1306 display;
};

class Junction  {
public:
  Junction(bool distanceToPillar, int setHeight, int junctionNumber);
  bool distance; 
  int height; 
  int number; 
};

#endif