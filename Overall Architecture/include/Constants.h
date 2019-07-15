#ifndef Constants_h
#define Constants_h

#define LEFT false
#define RIGHT true

//checking if the robot has been calibrated at least once 
#define YES_CALIBRATED 189763

//pwm_start
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 

//pins
#define WHEEL_ENCODER 0
#define ARM_SONAR 0
#define CLAW_ENCODER 0 //input
#define ARM_MOTOR_LEFT 0
#define ARM_MOTOR_RIGHT 0
#define CLAW_MOTOR_UP 0
#define CLAW_MOTOR_DOWN 0
#define ARM_HOME_SWITCH 0
#define ARM_SERVO 0
#define L_GAUNTLET_SERVO 0
#define R_GAUNTLET_SERVO 0
#define CLAW_SERVO 0
#define TUNING_KNOB_A 0
#define TUNING_KNOB_B 0
#define TUNING_BUTTON 0
#define L_TAPE_FOLLOW PA_3
#define R_TAPE_FOLLOW PA_4
#define L_SPLIT 0
#define R_SPLIT 0
#define L_TAB 0
#define R_TAB 0
#define L_GAUNTLET 0
#define R_GAUNTLET 0
#define LEFT_FORWARD_WHEEL_MOTOR PA_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_0
#define LEFT_BACKWARD_WHEEL_MOTOR 0
#define RIGHT_BACKWARD_WHEEL_MOTOR 0
#define CALIBRATE 0 //switch 
#define T_OR_M 0 //switch, HIGH = THANOS

// ManageStone library
//#define PILLAR_DISTANCE 0 // use inches 


// TapeFollower library
#define CHECKPT_A 0 //encoder clicks until first split
#define CHECKPT_B 0 //encoder clicks until 2nd split 
#define BOTTOM_DESTINATION 0
#define TOP_DESTINATION 0
#define SPEED_TUNING 4

// Tuning Menu 
#define NUM_VARIABLES 10

#endif

