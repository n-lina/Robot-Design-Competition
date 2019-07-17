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
   BACK_AWAY_FROM_EDGE,
   GO_HOME,
   PARK,
   NUM_STATES,
};

enum Calibration_Values {
  KP_WHEEL,
  KD_WHEEL,
  THRESHOLD,
  SPLIT_THRESHOLD,
  TAB_THRESHOLD,
  ALIGN_TAB_THRESHOLD,
  EDGE_THRESHOLD,
  COLLISION_THRESHOLD,
  PILLAR_DISTANCE,
  CALIBRATED_MAGIC,
  NUM_VARIABLES
};

class Robot{
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
   int KP_WHEEL; 
   int KD_WHEEL; 
   int THRESHOLD; 
   int SPLIT_THRESHOLD; 
   int TAB_THRESHOLD; 
   int ALIGN_TAB_THRESHOLD; 
   int EDGE_THRESHOLD; 
   int COLLISION_THRESHOLD;
   int PILLAR_DISTANCE; 
   Servo armServo; 
   Servo clawServo; 
   Servo L_GauntletServo;
   Servo R_GauntletServo;

private:
   Robot();  // Private so that it can  not be called
   Robot(Robot const& bot){};  // copy constructor is private
   Robot& operator=(Robot const& bot){}; // assignment operator is private
   static Robot* m_pInstance;
   void toggleMenu();
   void adjustVariables();
   int* CV_Addresses [NUM_VARIABLES];
   int CV_Values [NUM_VARIABLES];
   String labels [NUM_VARIABLES-1];
   volatile int value; 
   bool lastEncoderValue;
   bool encoderValue;
   Adafruit_SSD1306 display;
};

/*

------Robot.cpp-------------

#include "Robot.h"
// Global static pointer used to ensure a single instance of the class.
Robot* Robot::m_pInstance = NULL; 
 
// This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this Instance function.

   
Robot* Robot::i(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

Robot::Robot() : 
   lastEncoderValue(0), encoderValue(0), value(0),
    labels {"KP Tape Following","KD Tape Following","Line Following Threshold",
    "Split Detected Threshold","Gauntlet Tape Threshold","Edge Threshold", "Collision Threshold"},
    CV_Addresses {((int*) 0x0801FFF3), ((int*) 0x0801FFF7)}
{
}

-----------main.cpp------------------
#include "Robot.h"

setup(){
  Robot::i()->CV[CV_KPWHEEL];
}
}

----------- ThresholdMenu.cpp --------

[...]

value = *(Robot::i()->CV_Addresses[i]);

[...]

*(Robot::i()->CV[i]) = value;

-------------------------------------

*/
#endif