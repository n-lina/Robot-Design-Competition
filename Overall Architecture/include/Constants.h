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
#define L_ENCODER_A PB12
#define L_ENCODER_B PB13
#define R_ENCODER_A PB14
#define R_ENCODER_B PB15
#define COLLISION PB2
#define ARM_MOTOR_LEFT PA_8
#define ARM_MOTOR_RIGHT PA_10
#define ARM_MOTOR_UP PB_0
#define ARM_MOTOR_DOWN PB_1
#define ARM_SERVO PA7
#define CLAW_SERVO PA6
#define GAUNTLET_SERVO PA9
#define TUNING_KNOB PA13
#define L_ALIGN PA_0
#define L_DECIDE PA_1
#define L_TAPE_FOLLOW PA_2
#define R_TAPE_FOLLOW PA_3
#define R_DECIDE PA_4
#define R_ALIGN PA_5
#define LEFT_FORWARD_WHEEL_MOTOR PB_6
#define RIGHT_FORWARD_WHEEL_MOTOR PB_8
#define LEFT_BACKWARD_WHEEL_MOTOR PB_7
#define RIGHT_BACKWARD_WHEEL_MOTOR PB_9
#define T_OR_M PA14 //switch, HIGH = THANOS
#define MULTIPLEX_A PB3
#define MULTIPLEX_B PB4
#define MULTIPLEX_C PB5
#define MULTIPLEX_OUT PA15
#define COLLISION PB2
#define SONAR_ECHO PA12
#define SONAR_TRIG PA11
// ARM_HOME_LIMIT multi(0,1,0)
// ARM_SIDES_LIMIT multi(1,0,0)
// ARM_TOP_BOTTOM_LIMIT multi(0,0,1)
// NO_STONE multi(1,1,0)
// TUNING_BUTTON multi(1,0,1)
// COLLISION multi()
// TUNING_MENU/CALIBRATION YES NO multi(1,0,1);

// ManageStone library
#define THREE_INCHES 3000 //ms since 1 in/s

// TapeFollower library
#define SPEED_TUNING 1.2
#define TURN_DELAY_TIME 200
#define SOFT_TURN_DELAY_TIME 180
#define DEBOUNCE 4 // once junction detected, must be DEBOUNCE loops before next one 
#define ANGLE_START 70 
#define ANGLE_FINISH 140
#endif

