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
#define NOT_AVAIL -1

//checking if the robot has been calibrated at least once 
#define YES_CALIBRATED 189763

//pwm_start
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 

// PiINS
#define COLLISION PA15
// Servos
#define ARM_SERVO PA8
#define CLAW_SERVO PA9
#define GAUNTLET_SERVO PA10
// Phototransistors
#define L_ALIGN PA_0
#define L_DECIDE PA_1
#define L_TAPE_FOLLOW PA_2
#define R_TAPE_FOLLOW PA_3
#define R_DECIDE PA_4
#define R_ALIGN PA_5
// Motors
#define LEFT_FORWARD_WHEEL_MOTOR PB_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_8
#define LEFT_BACKWARD_WHEEL_MOTOR PB_7
#define RIGHT_BACKWARD_WHEEL_MOTOR PB_9
#define ARM_MOTOR_LEFT PB_1
#define ARM_MOTOR_RIGHT PB_0
#define ARM_MOTOR_UP PA_6
#define ARM_MOTOR_DOWN PA_7
// Setup
#define T_OR_M PA14 //switch, HIGH = THANOS
#define CALIBRATE PA13 //do we want to calibrate the robot
#define TUNING_KNOB_A PB12
#define TUNING_KNOB_B PB13
#define TUNING_BUTTON PB3
// Arm and claw
#define ARM_SIDES_LIMIT PA11
#define ARM_TOP_BOTTOM_LIMIT PA12
#define SONAR_ECHO PB14
#define SONAR_TRIG PB15


// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define SPEED_TUNING 1.8
#define TURN_DELAY_TIME 200
#define SOFT_TURN_DELAY_TIME 180
#define DEBOUNCE 1000 // once junction detected, must be DEBOUNCE loops before next one 
#define ANGLE_START 70 
#define ANGLE_FINISH 140
#define START_DETECTION 20000 //number of loops to run before starting junction detection 

#endif

