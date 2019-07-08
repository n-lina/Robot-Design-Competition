
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono12pt7b.h>
#include <Constants.h>


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif 

#define OLED_RESET -1  // Not used
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // Setting up OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  pinMode(TUNING_KNOB, INPUT_PULLUP);
  pinMode(TUNING_BUTTON, INPUT_PULLUP);

  // Making arrayw for variables to be tuned and their labels 
  int parameters [7] = {THRESHOLD, SPLIT_THRESHOLD, GAUNTLET_TAPE_THRESHOLD, KD_WHEEL, KP_WHEEL, 
  EDGE_THRESHOLD, COLLISION_THRESHOLD};
  String labels [7] = {"Tape Following Threshold", "Split Threshold", "Gauntlet Tape Threshold",
  "KD Tape Following", "KP Tape Follwing", "Edge Threshold", "Collision Thresshold"};

  for(int i=0; i<7; i++){
    display.println(labels[i]);
  }
}

void loop() {
  display.clearDisplay();  
  display.setCursor(0,0);
  display.println("Loop counter: ");
  display.println(loopcount);
  display.setCursor(0,15);
  display.println("Collisions: ");
  display.println(i);
  display.display();
}