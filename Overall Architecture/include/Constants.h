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
#define ARM_SONAR PA_0
#define CLAW_ENCODER PA_0
#define L_ENCODER PA_0
#define R_ENCODER PA_0
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
#define L_TAPE_FOLLOW PA_0
#define R_TAPE_FOLLOW PA_0
#define L_SPLIT PA_0
#define R_SPLIT PA_0
#define L_TAB PA_0
#define R_TAB PA_0
#define L_GAUNTLET PA_0
#define R_GAUNTLET PA_0
#define LEFT_FORWARD_WHEEL_MOTOR PA_0
#define RIGHT_FORWARD_WHEEL_MOTOR PA_0
#define LEFT_BACKWARD_WHEEL_MOTOR PA_0
#define RIGHT_BACKWARD_WHEEL_MOTOR PA_0
#define CALIBRATE PA_0 //switch 
#define T_OR_M PA_0 //switch, HIGH = METHANOS

PinName outputPins [12] ={ARM_SERVO, ARM_MOTOR_LEFT, ARM_MOTOR_RIGHT, CLAW_SERVO, CLAW_MOTOR_UP, CLAW_MOTOR_DOWN, 
L_GAUNTLET_SERVO, LEFT_FORWARD_WHEEL_MOTOR, RIGHT_FORWARD_WHEEL_MOTOR, R_GAUNTLET_SERVO, LEFT_BACKWARD_WHEEL_MOTOR, 
RIGHT_BACKWARD_WHEEL_MOTOR};

PinName inputPins [18] = {ARM_SONAR, CLAW_ENCODER, ARM_HOME_SWITCH, TUNING_KNOB_A, TUNING_KNOB_B, TUNING_BUTTON,
L_TAPE_FOLLOW, R_TAPE_FOLLOW, L_SPLIT, R_SPLIT, L_TAB, R_TAB, L_GAUNTLET, R_GAUNTLET, CALIBRATE, T_OR_M, 
L_ENCODER, R_ENCODER};
//input pullup or input?

// ManageStone library
#define PILLAR_DISTANCE 0 // use inches 
bool direction = LEFT; 

// TapeFollower library
volatile int* p_KP_WHEEL = (int*) 0x0801FFF3;
volatile int* p_KD_WHEEL = (int*) 0x0801FFF7;
volatile int* p_THRESHOLD = (int*) 0x0801FFFB;
volatile int* p_SPLIT_THRESHOLD = (int*) 0x0801FFEF;
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
