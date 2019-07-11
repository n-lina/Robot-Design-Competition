#ifndef Constants_h
#define Constants_h

#define LEFT true
#define RIGHT false
int state = 0;
bool TEAM = true; //true = THANOS, false = METHANOS; 

//checking if the robot has been calibrated at least once 
#define YES_CALIBRATED 189763

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
#define T_OR_M 0 //switch, HIGH = METHANOS

PinName outputPins [12] ={ARM_SERVO, ARM_MOTOR_LEFT, ARM_MOTOR_RIGHT, CLAW_SERVO, CLAW_MOTOR_UP, CLAW_MOTOR_DOWN, 
L_GAUNTLET_SERVO, LEFT_FORWARD_WHEEL_MOTOR, RIGHT_FORWARD_WHEEL_MOTOR, R_GAUNTLET_SERVO, LEFT_BACKWARD_WHEEL_MOTOR, 
RIGHT_BACKWARD_WHEEL_MOTOR};

PinName inputPins [16] = {ARM_SONAR, CLAW_ENCODER, ARM_HOME_SWITCH, TUNING_KNOB_A, TUNING_KNOB_B, TUNING_BUTTON,
L_TAPE_FOLLOW, R_TAPE_FOLLOW, L_SPLIT, R_SPLIT, L_TAB, R_TAB, L_GAUNTLET, R_GAUNTLET, CALIBRATE, T_OR_M};
//input pullup or input?

// ManageStone library
#define PILLAR_DISTANCE 0 // use inches 
bool direction = LEFT; 

// TapeFollower library
volatile int* p_KP_WHEEL = (int*) 0x0801FFF3;
volatile int* p_KD_WHEEL = (int*) 0x0801FFF7;
volatile int* p_THRESHOLD = (int*) 0x0801FFFB;
volatile int* p_SPLIT_THRESHOLD = (int*) 0x0801FFEF;
volatile int* p_TAB_THRESHOLD = (int*) 0x0801FFDB;
volatile int* p_GAUNTLET_TAPE_THRESHOLD = (int*) 0x0801FFEB;
volatile int* p_EDGE_THRESHOLD = (int*) 0x0801FFE7;
volatile int* p_COLLISION_THRESHOLD = (int*) 0x0801FFE3;
volatile int* p_CALIBRATED = (int*) 0x0801FFDF;

#endif

//SETUP 
//setup pins 
//pwm start motors 
//attach servos 
//create instances of classes 
//setup oled

/* //Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 //pinMode(L_MOTOR_BACKWARD, INPUT_PULLDOWN);
 pinMode(L_MOTOR_FORWARD, INPUT_PULLDOWN);
 //pinMode(R_MOTOR_BACKWARD, INPUT_PULLDOWN);
 pinMode(R_MOTOR_FORWARD, INPUT_PULLDOWN);
 pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
 pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);  */
