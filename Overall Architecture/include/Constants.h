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

#define BACKWARDS false
#define FORWARDS true

#define TAB true
#define SPLIT false

//pwm_start
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 

<<<<<<< HEAD
//pins
#define MULTIPLEX_A PB12 
#define MULTIPLEX_B PB13
#define DEMULTIPLEX_ARM_H PB14 
#define DEMULTIPLEX_ARM_V PB15
#define DEMULTIPLEX_R_WHEEL PB5
#define DEMULTIPLEX_L_WHEEL PB4

#define ARM_SERVO PA7
#define CLAW_SERVO PB0
#define GAUNTLET_SERVO PB1

#define ARM_TOP_BOTTOM_LIMIT PA11
#define ARM_SIDES_LIMIT PA12
#define COLLISION PA15
#define TUNING_BUTTON PB3

#define LEFT_WHEEL PB_8
#define RIGHT_WHEEL PB_9

#define ARM_MOTOR_H PA_8
#define ARM_MOTOR_V PA_10

#define R_ALIGN PA_5
#define R_DECIDE PA_4
#define R_TAPE_FOLLOW PA_3
#define L_TAPE_FOLLOW PA_2
#define L_DECIDE PA_1
#define L_ALIGN PA_0

#define T_OR_M PA13 //switch, HIGH = THANOS
=======
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
#define ARM_MOTOR_LEFT PA_6
#define ARM_MOTOR_RIGHT PA_7
#define ARM_MOTOR_UP PB_0
#define ARM_MOTOR_DOWN PB_1
// Setup
#define T_OR_M PA14 //switch, HIGH = THANOS
#define CALIBRATE PA13 //do we want to calibrate the robot
#define TUNING_KNOB_A PB12
#define TUNING_KNOB_B PB13
#define TUNING_BUTTON PB3
// Arm and claw
#define ARM_SIDES_LIMIT PA12
#define ARM_TOP_BOTTOM_LIMIT PA11
#define SONAR_ECHO PB14
#define SONAR_TRIG PB15

>>>>>>> master

// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define SPEED_TUNING 1.8
#define TURN_DELAY_TIME 200 
#define TURN_IN_PLACE_DELAY_TIME 400 // to ensure 180 degree turn 
#define DEBOUNCE 1000 // once junction detected, must be DEBOUNCE loops before next one 
#define ANGLE_START 70 
#define ANGLE_FINISH 140
#define START_DETECTION 30000 //number of loops to run before starting junction detection 
#define TOP_PATH_SIZE 4
#define BOTTOM_PATH_SIZE 6

#define _KP_WHEEL 162
#define _KD_WHEEL 12
#define _THRESHOLD 200
#define _DECIDE_THRESHOLD 300
#define _ALIGN_THRESHOLD 400

#endif

