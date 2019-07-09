#ifndef Setup_h
#define Setup_h
#include <Adafruit_SSD1306.h>
#include <FreeMono12pt7b.h>

class TuningMenu{
    public:
      TuningMenu();
      void adjustVariables();
    private: 
      void toggleMenu();
      volatile int* pointers[7];
      String labels[7];
      volatile int value; 
      bool lastEncoderValue;
      bool encoderValue;
      Adafruit_SSD1306 display;
}

class Setup{
    public:
      Setup();
      static void setup();
      static void methanosOrThanos(); 
}

#endif
