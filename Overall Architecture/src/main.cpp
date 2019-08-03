#include <Arduino.h>
#include "Robot.h"
#include "TapeFollower.h"
#include "ManageStone.h"
#include "Constants.h"
// #include <Wire.h>
// #include <Adafruit_SSD1306.h>
// #include <FreeMono9pt7b.h>

// #define OLED_RESET -1  // Not used
// Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define TUNING
//#define ALL_TOGETHER true
//#define COLLECT_STONE_X true //change X to be what stoneNumber u want, check the direction
// test no stone, choose stoneNumber 1,2, or 3 
//#define COLLECT_STONE_SONAR true
//#define GO_HOME true 
//#define MULTIPLEX

#ifdef ALL_TOGETHER 
TapeFollower robot(Robot::instance());
ManageStone Robot(Robot::instance());

void setup() {
 Robot::instance()->setup();
 Serial.begin(9600);
}

void loop() {
  switch (Robot::instance()->state){
    // case GO_DISTANCE: // Go a certain distance without checking for tabs/splits
    //   //TODO
    //   break;
    case FOLLOW_TAPE: // Follow Tape checking splits/tabs
      robot.followTape();
      break;
    case AVOID_COLLISION: // Avoid Collision {4}
      //TODO // no more collision sonar 
      break; 
    case SPLIT_CHOOSER: // Split Chooser 
      robot.splitDecide();
      break;
    case COLLECT_STONE: // Collect Stone {3}
      Robot.collectStone();
      break;
    case GO_HOME: // Go home and deposit stones at certain time (1:30) {2} turn off collision interrupts 
      robot.goHome();
      break;
    case PARK: // park at gauntlet
      robot.park();
      break;
  }
}
#endif

#ifdef COLLECT_STONE_X
ManageStone Robot(Robot::instance());

void setup() {
  Serial.begin(9600);
  Robot::instance()->setup();
  Robot::instance()->direction = LEFT;
  Robot.collectStone();
}

void loop (){
}
#endif

#ifdef GO_HOME
TapeFollower robot(Robot::instance());

void setup() {
  Robot::instance()->setup();
}

void loop() {
  robot.goHome(true);
}
#endif

#ifdef MULTIPLEX 
int curr_val;
bool multi(bool A, bool B, bool C);

void setup(){
  Serial.begin(9600);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(MULTIPLEX_C, OUTPUT);
  pinMode(MULTIPLEX_OUT, INPUT);
}

void loop(){
  //Select each pin and read value
  curr_val = multi(1, 0, 0);
  Serial.println(curr_val);
  delay(2000);
}

bool multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C);

  bool multi_out = digitalRead(MULTIPLEX_OUT);  
  return multi_out;
}

#endif 

#ifdef TUNING 
void setup(){
  Robot::instance()->setup();
  delay(3000);
  Serial.begin(9600);
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done

  // Clear the buffer.
  // display.clearDisplay();

  // // Draw a test
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(0,0);
  // display.println("OLED Display 128x64");
  // display.setFont(&FreeMono9pt7b);
  // display.drawPixel(0,45,WHITE);
  // display.setCursor(4,45);
  // display.println("Welcome!");
  // display.display();
  // delay(1000);
}
void loop(){
  // display.clearDisplay();
  // display.setCursor(4,45);
  // display.println("TESTING");
  // display.display();
  Serial.println("Entered loop");
  delay(1000);
  // Robot::instance()->adjustVariables();
  Robot::instance()->display.println("hello");
  Robot::instance()->display.display();
  delay(1000);
  Serial.println("Exiting loop");
}
#endif