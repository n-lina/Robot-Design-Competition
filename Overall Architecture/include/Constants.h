#ifndef CollectStone_h
#define CollectStone_h

#define LEFT true
#define RIGHT false

//pwm_start
#define MAX_SPEED 1024 
#define CLOCK_FQ 100000 

//pins
#define ARM_SONAR 0
#define CLAW_ENCODER 0 //input
#define ARM_MOTOR_LEFT 0
#define ARM_MOTOR_RIGHT 0
#define CLAW_MOTOR_UP 0
#define CLAW_MOTOR_DOWN 0
#define ARM_HOME_SWITCH 0
#define ARM_SERVO 0

// ManageStone library
#define PILLAR_DISTANCE 0 // use inches 
#define KP_ARM 0
bool direction = LEFT; 
#endif

//SETUP 
//setup pins 
//pwm start motors 
//attach servos 