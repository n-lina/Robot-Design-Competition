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
#define L_WHEEL_ENCODER PA_0
#define R_WHEEL_ENCODER PA_0
#define ARM_SONAR_ECHO PB5 
#define ARM_SONAR_TRIGGER PB14
#define ARM_MOTOR_LEFT PB_1
#define ARM_MOTOR_RIGHT PB_0
#define ARM_MOTOR_UP PA_9
#define ARM_MOTOR_DOWN PA_10
#define ARM_HOME_SWITCH PA_0
#define ARM_SERVO PB3
#define CLAW_SERVO PA15
#define L_GAUNTLET_SERVO PA0
#define R_GAUNTLET_SERVO PA0
#define TUNING_KNOB_A PA_0
#define TUNING_KNOB_B PA_0
#define TUNING_BUTTON PA_0
#define L_TAB PA_0
#define L_SPLIT PA_1
#define L_TAPE_FOLLOW PA_2
#define R_TAPE_FOLLOW PA_3
#define R_SPLIT PA_4
#define R_TAB PA_5
#define L_TAB_ALIGN PA_0
#define R_TAB_ALIGN PA_0
#define MIDDLE_TAB_ALIGN PA_0
#define LEFT_FORWARD_WHEEL_MOTOR PB_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_9
#define LEFT_BACKWARD_WHEEL_MOTOR PB_7
#define RIGHT_BACKWARD_WHEEL_MOTOR PB_8
#define CALIBRATE PA_0 //switch 
#define T_OR_M PA_0 //switch, HIGH = THANOS
#define ARM_LIMIT PA_0
#define NO_STONE PA_0
#define MULTIPLEX_A PA7
#define MULTIPLEX_B PA6
#define MULTIPLEX_C PA8
#define MULTIPLEX_OUT PB15


// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define CHECKPT_A 0 //encoder clicks until first split
#define CHECKPT_B 0 //encoder clicks until 2nd split 
#define BOTTOM_DESTINATION 0
#define TOP_DESTINATION 0
#define SPEED_TUNING 1.2
#define TURN_DELAY_TIME 300
#define SOFT_TURN_DELAY_TIME 180

#endif

