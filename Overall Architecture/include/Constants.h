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
#define L_WHEEL_ENCODER PB12
#define R_WHEEL_ENCODER PB13
#define ARM_SONAR_ECHO PB5 
#define ARM_SONAR_TRIGGER PB14
#define COLLISION PB2
#define ARM_MOTOR_LEFT PB_0
#define ARM_MOTOR_RIGHT PB_1
#define ARM_MOTOR_UP PA_8
#define ARM_MOTOR_DOWN PA_10
#define ARM_SERVO PB3
#define CLAW_SERVO PA4
#define GAUNTLET_SERVO PA15
#define TUNING_KNOB PA_11
#define L_TAB PA_0
#define L_SPLIT PA_1
#define L_TAPE_FOLLOW PA_2
#define R_TAPE_FOLLOW PA_3
#define R_SPLIT PA_4
#define R_TAB PA_5
#define LEFT_FORWARD_WHEEL_MOTOR PB_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_8
#define LEFT_BACKWARD_WHEEL_MOTOR PB_7
#define RIGHT_BACKWARD_WHEEL_MOTOR PB_9
#define CALIBRATE PA13 //switch 
#define T_OR_M PA14 //switch, HIGH = THANOS
#define MULTIPLEX_A PA7
#define MULTIPLEX_B PA6
#define MULTIPLEX_C PA12
#define MULTIPLEX_OUT PB15
// ARM_HOME_LIMIT multi(0,1,0)
// ARM_SIDES_LIMIT multi(1,0,0)
// ARM_TOP_BOTTOM_LIMIT multi(0,0,1)
// NO_STONE multi(1,1,0)
// TUNING_BUTTON multi(1,0,1)
// COLLISION multi()

// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define SPEED_TUNING 1.2
#define TURN_DELAY_TIME 300
#define SOFT_TURN_DELAY_TIME 180

// Time trials no tuning menu 
#define KP_WHEEL_ 164
#define KD_WHEEL_ 11 
#define THRESHOLD_ 300 
#define SPLIT_THRESHOLD_ 480 
#define TAB_THRESHOLD_ 200
#define PILLAR_DISTANCE_ 7 //cm  

#endif

