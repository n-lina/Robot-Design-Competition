#include <Arduino.h>
#include <Servo.h>
#include "Robot.h"
#include "Constants.h"

Robot* Robot::m_pInstance = NULL; 

Robot::Robot(): 
  state(GO_DISTANCE), 
  TEAM(digitalRead(T_OR_M) == HIGH), 
  THRESHOLD(_THRESHOLD),
  ALIGN_THRESHOLD(_ALIGN_THRESHOLD),
  DECIDE_THRESHOLD(_DECIDE_THRESHOLD),
  stoneNumber(0), 
  collisionNumber(0), 
  junctionNumber(0), 
  direction_facing(true), 
  direction(), 
  armServo(), 
  clawServo(), 
  L_GauntletServo(), 
  R_GauntletServo()
 // display(Adafruit_SSD1306(-1))
{
}

Robot* Robot::instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new Robot;
   return m_pInstance;
}

void Robot::setup(){
  pinMode(ARM_TOP_BOTTOM_LIMIT, INPUT);
  pinMode(ARM_SIDES_LIMIT, INPUT);
  pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP);
  pinMode(T_OR_M, INPUT);
  pinMode(COLLISION, INPUT_PULLUP);
  /////////////////////////////////////
  pinMode(ARM_MOTOR_H, OUTPUT);
  pinMode(ARM_MOTOR_V, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(GAUNTLET_SERVO, OUTPUT);
  pinMode(RIGHT_WHEEL, OUTPUT);
  pinMode(LEFT_WHEEL, OUTPUT);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(DEMULTIPLEX_ARM_H, OUTPUT);
  pinMode(DEMULTIPLEX_ARM_V, OUTPUT);
  pinMode(DEMULTIPLEX_R_WHEEL, OUTPUT);
  pinMode(DEMULTIPLEX_L_WHEEL, OUTPUT);
  // // pwm_start init motors
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_V, CLOCK_FQ, MAX_SPEED, 0, 1);

  // //   // Attaching servos 
  armServo.attach(ARM_SERVO);
  clawServo.attach(CLAW_SERVO);
  L_GauntletServo.attach(GAUNTLET_SERVO);
  R_GauntletServo.attach(GAUNTLET_SERVO);

  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  // while(digitalRead(CALIBRATE)){
  //     display.clearDisplay();
  //     display.setCursor(0,0);
  //     display.println("Threshold: " + String(multi(0,0)));
  //     display.println("Fork Threshold: " + String(multi(0,1)));
  //     display.println("Align Threshold: " + String(multi(1,0)));
  //     display.display();      
  // }
  // THRESHOLD = multi(0,0);
  // ALIGN_THRESHOLD = multi(1,0);
  // DECIDE_THRESHOLD = multi(0,1);

  clawServo.write(0);
  L_GauntletServo.write(70);

  if(THANOS){
    armServo.write(180);
  }
  else{
    armServo.write(0);
  }

  if(THANOS){
    digitalWrite(DEMULTIPLEX_ARM_H, LOW);
    pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  else{
    digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
    pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
}

// bool Robot::multi(bool A, bool B){
//   digitalWrite(MULTIPLEX_A, A);
//   digitalWrite(MULTIPLEX_B, B);
//   return analogRead(MULTIPLEX_OUT);
// }








