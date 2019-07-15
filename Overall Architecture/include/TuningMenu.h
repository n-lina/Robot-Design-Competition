#ifndef TuningMenu_h
#define TuningMenu_h

#include <Adafruit_SSD1306.h>
#include <FreeMono12pt7b.h>

class TuningMenu{
    public:
      TuningMenu();
      void adjustVariables();
      void loadVariables();
      volatile int* p_KP_WHEEL;
      volatile int* p_KD_WHEEL;
      volatile int* p_THRESHOLD;
      volatile int* p_SPLIT_THRESHOLD;
      volatile int* p_TAB_THRESHOLD;
      volatile int* p_GAUNTLET_TAPE_THRESHOLD;
      volatile int* p_EDGE_THRESHOLD;
      volatile int* p_COLLISION_THRESHOLD;
      volatile int* p_PILLAR_DISTANCE; 
      volatile int* p_CALIBRATED;
    private: 
      void toggleMenu();
      String labels[7];
      volatile int value; 
      bool lastEncoderValue;
      bool encoderValue;
      Adafruit_SSD1306 display;
};

#endif

//maybe tune set distances based on M/T if using encoders to get to the destination 