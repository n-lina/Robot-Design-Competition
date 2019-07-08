#ifndef Constants_h
#define Constants_h

#define LEFT true
#define RIGHT false
int state = 0;

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
#define GAUNTLET_SERVO 0
#define TUNING_KNOB 0
#define TUNING_BUTTON 0
#define L_TAPE_FOLLOW 0
#define R_TAPE_FOLLOW 0
#define L_SPLIT 0
#define R_SPLIT 0
#define L_TAB 0
#define R_TAB 0
#define LEFT_FORWARD_WHEEL_MOTOR 0
#define RIGHT_FORWARD_WHEEL_MOTOR 0
#define LEFT_BACKWARD_WHEEL_MOTOR 0
#define RIGHT_BACKWARD_WHEEL_MOTOR 0

// ManageStone library
#define PILLAR_DISTANCE 0 // use inches 
bool direction = LEFT; 

// TapeFollower library
int KP_WHEEL = 30; // PID proportion constant // 205 is  too high 
int KD_WHEEL = 9; // PID derivative constant 
int THRESHOLD = 200; // Threshold for being on or off the line
int SPLIT_THRESHOLD = 200; // "" for splits 
int GAUNTLET_TAPE_THRESHOLD = 200; // "" for gauntlet tapes 

int EDGE_THRESHOLD = 0;
int COLLISION_THRESHOLD = 0;

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
