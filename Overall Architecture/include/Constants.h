#ifndef Constants_h
#define Constants_h

#define LEFT false
#define RIGHT true

#define SPLIT false
#define TAB true 

#define SMALL false 
#define LARGE true

#define METHANOS false 
#define THANOS true 

#define CALIBRATE HIGH
#define NO_CALIBRATE LOW

#define NOT_AVAIL -1

//checking if the robot has been calibrated at least once 
#define YES_CALIBRATED 189763

//pwm_start
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 

//pins
#define TUNING_KNOB_A PB12
#define TUNING_KNOB_B PB13
#define SONAR_TRIG PB14
#define SONAR_ECHO PB15

#define ARM_SERVO PA8
#define CLAW_SERVO PA9
#define GAUNTLET_SERVO PA10

#define ARM_TOP_BOTTOM_LIMIT PA11
#define ARM_SIDES_LIMIT PA12
#define COLLISION PA15
#define TUNING_BUTTON PB3

#define LEFT_FORWARD_WHEEL_MOTOR PB_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_8
#define LEFT_BACKWARD_WHEEL_MOTOR PB_7
#define RIGHT_BACKWARD_WHEEL_MOTOR PB_9

#define ARM_MOTOR_LEFT PB_1
#define ARM_MOTOR_RIGHT PB_0
#define ARM_MOTOR_DOWN PA_7
#define ARM_MOTOR_UP PA_6

#define R_ALIGN PA_5
#define R_DECIDE PA_4
#define R_TAPE_FOLLOW PA_3
#define L_TAPE_FOLLOW PA_2
#define L_DECIDE PA_1
#define L_ALIGN PA_0

#define T_OR_M PA14 //switch, HIGH = THANOS
#define CALIBRATE PA13 //do we want to calibrate the robot

// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define SPEED_TUNING 1.2
#define TURN_DELAY_TIME 200
#define SOFT_TURN_DELAY_TIME 180
#define DEBOUNCE 5 // once junction detected, must be DEBOUNCE loops before next one 
#define ANGLE_START 70 
#define ANGLE_FINISH 140
#define START_DETECTION 30000 //number of loops to run before starting junction detection 

#define _KP_WHEEL 162
#define _KD_WHEEL 12
#define _THRESHOLD 200
#define _DECIDE_THRESHOLD 300
#define _ALIGN_THRESHOLD 400
#define _PILLAR_DISTANCE 4

#endif

