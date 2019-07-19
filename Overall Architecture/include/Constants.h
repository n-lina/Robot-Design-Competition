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
#define WHEEL_ENCODER PA_0
#define ARM_SONAR PA_0
#define CLAW_ENCODER PA_0 //input
#define ARM_MOTOR_LEFT PA_0
#define ARM_MOTOR_RIGHT PA_0
#define CLAW_MOTOR_UP PA_0
#define CLAW_MOTOR_DOWN PA_0
#define ARM_HOME_SWITCH PA_0
#define ARM_SERVO PA_0
#define L_GAUNTLET_SERVO PA_0
#define R_GAUNTLET_SERVO PA_0
#define CLAW_SERVO PA_0
#define TUNING_KNOB_A PA_0
#define TUNING_KNOB_B PA_0
#define TUNING_BUTTON PA_0
#define L_TAPE_FOLLOW PA_3
#define R_TAPE_FOLLOW PA_4
#define L_SPLIT PA_0
#define R_SPLIT PA_0
#define L_TAB PA_0
#define R_TAB PA_0
#define L_TAB_ALIGN PA_0
#define R_TAB_ALIGN PA_0
#define MIDDLE_TAB_ALIGN PA_0
#define LEFT_FORWARD_WHEEL_MOTOR PA_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_0
#define LEFT_BACKWARD_WHEEL_MOTOR PA_0
#define RIGHT_BACKWARD_WHEEL_MOTOR PA_0
#define CALIBRATE PA_0 //switch 
#define T_OR_M PA_0 //switch, HIGH = THANOS
#define ARM_LIMIT PA_0
#define ARM_LIMIT PA_0
#define NO_STONE PA_0


// ManageStone library
#define NINE_INCH_TIME 1000 //ms
#define TWELVE_INCH_TIME 2000 //ms

// TapeFollower library
#define CHECKPT_A 0 //encoder clicks until first split
#define CHECKPT_B 0 //encoder clicks until 2nd split 
#define BOTTOM_DESTINATION 0
#define TOP_DESTINATION 0
#define SPEED_TUNING 4

#endif

