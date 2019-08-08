#include <Arduino.h>
#include <Servo.h>
#include "Constants.h"


#define GO true
//#define SENSORS true
//#define MOTORS true
//#define SPLIT_DECIDE true
//#define TURN_IN_PLACE true
//#define DROP_OFF_STONE true
//#define CHECK_TEAM true
//#define CHECK_LIMIT true

#ifdef GO
int derivative; 
int timeStep=0; 
int lastError=0; 
int position; 
int lastPosition=0; 
int PID; 
int default_speed = MAX_SPEED;
int number=0;
bool leftTapeFollow;
bool rightTapeFollow;
bool leftDecide;
bool rightDecide;
bool leftAlign;
bool rightAlign;
int debounce = 0;
int junctionNumber = 0;
int loopCounter = 0;
int stoneCollected = 0;
bool pressed = false;
bool done = false;
bool TEAM;
Servo clawServo; 
Servo armServo; 
Servo cottonCandy;
  

void turnLeft();
void turnRight();
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();
void alignPillar();
void alignPillarLeft();
void alignPillarRight();
void getPosition();
void collectStone();
void dropOffStone();
void deployCottonCandy();
void turnInPlaceRightGuantlet();
void turnInPlaceLeftGuantlet();
void setupRobot();

void setup()
{
  setupRobot();
}

void loop(){

  if(!done && loopCounter < START_DETECTION){
    if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pressed = true;
    }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>= _THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>= _THRESHOLD;
    timeStep++;

    if (leftTapeFollow  && rightTapeFollow ){
      position = 0;
    } 
    else if(leftTapeFollow  && !rightTapeFollow ){
      position = 1; 
    }  
    else if(!leftTapeFollow  && rightTapeFollow ){
      position = -1; 
    }
    else{
      if(lastPosition<0) { //right was on last 
      position = -5; 
      }
      else { // last Position > 0 ==> left was on last 
      position = 5;
      }   
    } 
    derivative = (position - lastPosition) / timeStep; 
    PID = (_KP_WHEEL * position) + (_KD_WHEEL * derivative); 
    
    pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0);
    pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0); 

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
  }

else{
  done = true;    //done = start detecting

  if(!pressed && digitalRead(ARM_SIDES_LIMIT) == HIGH){
    pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pressed = true;
  }

  leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= _THRESHOLD;
  rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= _THRESHOLD;
  leftDecide = analogRead(L_DECIDE) >= _DECIDE_THRESHOLD;
  rightDecide = analogRead(R_DECIDE) >= _DECIDE_THRESHOLD;

  if((leftDecide || rightDecide) && (debounce > DEBOUNCE)){
    junctionNumber++;
    debounce = 0;
    switch(junctionNumber){
      case 1: 
        if(TEAM == METHANOS){
          stop();
          turnRight();
        }
        else{
          stop();
          turnLeft();
        }
        break;
      case 2: 
      if(TEAM == METHANOS){
        if(leftDecide || (analogRead(L_DECIDE) > _DECIDE_THRESHOLD)){
          turnLeft();
        }
      }
      else{
        if(rightDecide || (analogRead(R_DECIDE) > _DECIDE_THRESHOLD)){
          turnRight();
        }
      }
        break;
      case 3:
        break;
      case 4:
        stop();
        delay(1000);
        alignPillar();
        collectStone();
        dropOffStone();
        stop();
        delay(500);
        pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 600, 0);
        pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
        delay(500);
        turnInPlaceRight();
        default_speed = 700;
        break;

      case 5: 
        pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 600, 0);
        pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 800, 0);
        delay(30);
        default_speed = MAX_SPEED;
        break;

      case 6: 
        stop();
        pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
        pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
        delay(500);
        turnInPlaceRightPillar();
        break;

      case 7:
        stop();
        delay(1000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(3000);
        pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0 ,0);
        clawServo.write(0);
        delay(500);
        cottonCandy.write(70);
        delay(500);
        armServo.write(0);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
        delay(3000);
        pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
        alignPillarRight();
        collectStone();
        dropOffStone();
        stop();
        delay(500);
        turnInPlaceRightPillar();

      case 8:
        break;
      
      case 9:
        stop();
        delay(500);
        default_speed = 280;
        // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
        // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
        //delay(300);
        if(METHANOS){
          turnLeft();
          stop();
          delay(200);
          turnInPlaceRightGuantlet();
        }
        else{
          turnInPlaceLeftGuantlet();
        }
        break;

      case 10:
        pwm_start(L_MOTOR_FORWARD,CLOCK_FQ, MAX_SPEED, 280, 0);
        pwm_start(R_MOTOR_FORWARD,CLOCK_FQ, MAX_SPEED, 265, 0);
        delay(500);
        stop();
        delay(1000);
        deployCottonCandy();
        
        break;

      // case 7:
      //   stop();
      //   if(METHANOS){
      //     turnInPlaceLeft();
      //   }
      //   else{
      //     turnInPlaceRight();
      //   }
      //   break;

      // case 8: 
      //   stop();
      //   alignPillar();
      //   stop();
      //   delay(500);
      //   //collectStone();
      //   //dropOffStone();
      //   if(METHANOS){
      //     turnInPlaceLeftPillar();
      //   }
      //   else{
      //     turnInPlaceRightPillar();
      //   }
      //   // turnLeft();
      //   // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
      //   // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
      //   // delay(500);
      //   // turnInPlaceRight();
      //   break;

      // case 9: 
      //   if(METHANOS){
      //     turnLeft();
      //   }
      //   else{
      //     turnRight();
      //   }
      //   break;

      // case 10:
      //   if(METHANOS){
      //     turnLeft();
      //   }
      //   else{
      //     turnRight();
      //   }
      //   pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
      //   pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
      //   delay(300);
      //   if(METHANOS){
      //     turnInPlaceRightPillar();
      //   }
      //   else{
      //     turnInPlaceLeftPillar();
      //   }
      //   break;

      // case 11:
      // if(METHANOS){
      //   //if(leftDecide){
      //     turnLeft();
      //   //}
      // }
      // else{
      //   //if(rightDecide){
      //     turnRight();
      //   //}
      // }
      //   break;

      // default:    //park at gauntlet and deploy cotton candy
      //   //pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      //   //pwm_stmart(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      //   //delay(1000);
      //   deployCottonCandy();
      //   break;
    }
  }

  timeStep++;

  if (leftTapeFollow && rightTapeFollow){
    position = 0;
  } 
  else if(leftTapeFollow  && !rightTapeFollow ){
    position = 1; 
  }
  else if(!leftTapeFollow && rightTapeFollow){
    position = -1; 
  }
  else{
    if(lastPosition<0) { //right was on last 
      position = -5; 
    }
    else { // last Position > 0 ==> left was on last 
      position = 5;
    } 
  }

 // L on, R off : position = positive, want to turn LEFT 
 // R on, L off : position = negative, want to turn RIGHT 
  derivative = (position - lastPosition) / timeStep; 
  PID = (KP * position) + (KD * derivative); 

  if(PID>(default_speed)){
    PID = default_speed;
  }

  else if(PID<-default_speed){
    PID = -default_speed;
  }
 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0);
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0); 
 //input of 18 still runs

  if(lastPosition != position){
    number++;
    if(number==2){
      timeStep = 0;
      number = 0;
      }
    }
  lastPosition = position; 
  debounce++;
  }
}

void turnInPlaceLeft(){      //turn in place for a longer period
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 300, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0); 
  delay(800);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      delay(50);
      debounce = 500;
      //turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){      //turn in place for a longer period
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 250, 0); 
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 250, 0); 
  delay(800);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      //turnLeft();
      //debounce = 500;
      return;
    }
  }
}

void turnInPlaceRightGuantlet(){      //turn in place slowly
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(600);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      delay(50);
      //turnLeft();
      return;
    }
  }
}

void turnInPlaceLeftGuantlet(){      //turn in place slowly
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(600);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      delay(50);
      //turnRight();
      return;
    }
  }
}

void stop(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
}

void turnLeft(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 1000, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

void turnRight(){
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 1000, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

void alignPillar(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD || analogRead(R_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      return;
    }
  }
  return;
}

void alignPillarLeft(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0); //very slow // best case: 70
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 250, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD || analogRead(R_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      return;
    }
  }
  return;
}

void alignPillarRight(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 250, 0); //very slow // best case: 70
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD || analogRead(R_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      return;
    }
  }
  return;
}

void getPosition(){
  leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= _THRESHOLD;
  rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= _THRESHOLD;
    if (leftTapeFollow  && rightTapeFollow ){
      position = 0;
    } 
    else if(leftTapeFollow  && !rightTapeFollow){
      position = 1; 
    }
    else if(!leftTapeFollow && rightTapeFollow ){
      position = -1; 
    }
    else{
      if(lastPosition<0) { //right was on last 
        position = -5; 
      }
      else { // last Position > 0 ==> left was on last 
        position = 5;
      } 
    }
  lastPosition = position; 
}

void collectStone(){
  stoneCollected++;
  clawServo.write(0);
  if(METHANOS){
    armServo.write(180);
  }
  else{
    armServo.write(0);
  }
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(4000);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  clawServo.write(180); //deploying claw
  delay(850); // waiting for it to grasp
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); // lifting stone out of the hole 
  delay(4000);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  delay(100);
  // armServo.write(180);
  // delay(500);
  //clawServo.write(0);
  //delay(3000);
  return;  
}

void dropOffStone(){
  if(THANOS){
    //claw is still closed before entering function
    //when claw is on right side of bot
    if(stoneCollected == 2){
      cottonCandy.write(100);
      armServo.write(80);
      delay(500);
      clawServo.write(0);
      delay(500);
      cottonCandy.write(70);
      //delay(500);
      armServo.write(180);  
    }

    //second right channel
    else if(stoneCollected == 1){
      cottonCandy.write(100);
      armServo.write(94);
      delay(500);
      clawServo.write(0);
      delay(500);
      cottonCandy.write(70);
      delay(500);
      armServo.write(180);

      // cottonCandy.write(130);
      // delay(1000);
      // cottonCandy.write(70);
      // delay(1000);
    }

    else if(stoneCollected == 3){
      pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);      //move the arm to the left for a bit, then drop it off
      delay(2000);
      cottonCandy.write(100);
      armServo.write(90);
      delay(500);
      clawServo.write(0);
      delay(500);
      cottonCandy.write(70);
      delay(500);
      armServo.write(180);
    }
  }

  //when claw is on left side of bot
  else{
    if(stoneCollected == 2){
      cottonCandy.write(100);
      armServo.write(89);
      delay(500);
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(6000);
      pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
      clawServo.write(0);
      delay(500);
      cottonCandy.write(70);
      armServo.write(0);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
      delay(6000);
      pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
    }

    //second left channel
    else if(stoneCollected == 1){
      cottonCandy.write(100);
      armServo.write(75);
      delay(500);
      // clawServo.write(0);
      // delay(500);
      // cottonCandy.write(70);
      // delay(500);
      // armServo.write(0);
    }

    else if(stoneCollected == 3){
      //move arm slightly to right and move to the second right one
      pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);      //move the arm to the left for a bit, then drop it off
      delay(2000);
      cottonCandy.write(100);
      armServo.write(90);
      delay(500);
      clawServo.write(0);
      delay(500);
      cottonCandy.write(70);
      delay(500);
      armServo.write(0);
    }
  }
}

void deployCottonCandy(){
  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(140);
  delay(1000);
  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(140);
  delay(1000);
  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(140);
  delay(1000);
  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(140);
  delay(1000);
  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(140);
  delay(1000);
  cottonCandy.write(150);
  delay(1000);
}


void setupRobot(){
  pinMode(ARM_SIDES_LIMIT, INPUT);
  pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);
  pinMode(L_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_TAPE_FOLLOW, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP);
  pinMode(T_OR_M, INPUT);
  pinMode(COLLISION, INPUT_PULLUP);
  /////////////////////////////////////
  pinMode(ARM_MOTOR_RIGHT, OUTPUT);
  pinMode(ARM_MOTOR_LEFT, OUTPUT);
  pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(PB15, OUTPUT);
  pinMode(GAUNTLET_SERVO, OUTPUT);
  pinMode(RIGHT_WHEEL_FORWARD, OUTPUT);
  pinMode(LEFT_WHEEL_FORWARD, OUTPUT);
  pinMode(RIGHT_WHEEL_BACKWARD, OUTPUT);
  pinMode(LEFT_WHEEL_BACKWARD, OUTPUT);

  // // pwm_start init motors
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1);

     //Attaching servos 
  armServo.attach(ARM_SERVO);
  clawServo.attach(CLAW_SERVO);
  cottonCandy.attach(GAUNTLET_SERVO);

  clawServo.write(0);
  cottonCandywrite(100);

  if(digitalRead(T_OR_M) == HIGH){
    TEAM = METHANOS; 
    armServo.write(0);
    pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }
  else{
    TEAM = THANOS; 
    armServo.write(180);
    pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  }

  digitalWrite(PB15, LOW);
}

#endif

#ifdef SENSORS 
void setup(){
  pinMode(PHOTO_0, INPUT_PULLUP);
  pinMode(PHOTO_1, INPUT_PULLUP);
  pinMode(R_ALIGN, INPUT_PULLUP);
  pinMode(R_DECIDE, INPUT_PULLUP);
  pinMode(L_DECIDE, INPUT_PULLUP);
  pinMode(L_ALIGN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop(){
 Serial.println("Right Align: ");
 Serial.println(String(analogRead(R_ALIGN)));
 Serial.println("Right Decide: ");
 Serial.println(String(analogRead(R_DECIDE)));
 Serial.println("Right Photo: ");
 Serial.println(String(analogRead(PHOTO_1)));
 Serial.println("Left Photo: ");
 Serial.println(String(analogRead(PHOTO_0)));
 Serial.println("Left Decide: ");
 Serial.println(String(analogRead(L_DECIDE)));
 Serial.println("Left Align: ");
 Serial.println(String(analogRead(L_ALIGN)));
 Serial.println("________________");
 delay(2000);
}
#endif


#ifdef MOTORS 

void stop();

void setup(){
pinMode(L_MOTOR_FORWARD, OUTPUT);
pinMode(L_MOTOR_BACKWARD, OUTPUT);
pinMode(R_MOTOR_FORWARD, OUTPUT);
pinMode(R_MOTOR_BACKWARD, OUTPUT);
pinMode(ARM_LEFT, OUTPUT);
pinMode(ARM_RIGHT, OUTPUT);
pinMode(ARM_MOTOR_UP, OUTPUT);
pinMode(ARM_MOTOR_DOWN, OUTPUT);
pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 1);
pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, 0, 1); 
pwm_start(ARM_MOTOR_DOWN, CLOCK_FQ, MAX_SPEED, 0, 1); 
Serial.begin(9600);
}

void loop(){
  // pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  // pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  // delay(2000);
  // stop();
  // delay(2000);
  // pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  // pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
  // delay(2000);
  // stop();
  // delay(2000);

  pwm_start(ARM_MOTOR_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  
}

void stop(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
}
#endif

#ifdef SPLIT_DECIDE
bool THANOS = false; 
int derivative; 
int timeStep=0; 
int position; 
int lastPosition=0; 
int PID; 
int number;
bool leftSensor;
bool rightSensor;
bool leftTab; 
bool rightTab;
bool leftSplit; 
bool rightSplit;
bool pressed = false;
int loopCounter =0;
int splitNumber =0;
int default_speed = MAX_SPEED/SPEED_TUNING;

void turnLeft();
void turnRight();
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();
void collectStone();
void dropGauntlet();

Servo armServo; 
Servo clawServo; 
Servo gauntletServo;

void setup()
{
 Serial.begin(9600);
 pinMode(PHOTO_0, INPUT_PULLUP);
 pinMode(PHOTO_1, INPUT_PULLUP);
 pinMode(L_SPLIT, INPUT_PULLUP);
 pinMode(R_SPLIT, INPUT_PULLUP);
  pinMode(CLAW_UP, OUTPUT);
  pinMode(CLAW_DOWN, OUTPUT);
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(CLAW_SERVO, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(MULTIPLEX_A, OUTPUT);
  pinMode(MULTIPLEX_B, OUTPUT);
  pinMode(MULTIPLEX_C, OUTPUT);
  pinMode(MULTIPLEX_OUT, INPUT);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO); 
  gauntletServo.attach(GAUNTLET);
  pinMode(GAUNTLET, OUTPUT);
  gauntletServo.attach(GAUNTLET);
 splitNumber = 0;
 gauntletServo.write(90);
 
 if(THANOS){
   pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 }
 else{
   pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
 }
}

void loop(){
  leftSensor = analogRead(PHOTO_0) >= THRESHOLD;
  rightSensor = analogRead(PHOTO_1) >= THRESHOLD;
  leftSplit =  analogRead(L_SPLIT)>=TAB_THRESHOLD;
  rightSplit = analogRead(R_SPLIT)>=TAB_THRESHOLD;
  
  if((leftSplit || rightSplit) && (rightSensor || leftSensor)){
    splitNumber++;
    switch(splitNumber){
      case 1: case 2:
        if(THANOS){
          turnLeft();
        }
        else{
          turnRight();
        }
        break;
      case 3: case 4: case 5: case 6: case 7: case 8:
        break; 
      case 9: 
        delay(50);
        stop();
        collectStone();
        delay(5000);
        dropGauntlet();
        break;
    }
  }

  if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
    pressed = true;
  }
   
 timeStep++;

 if (leftSensor && rightSensor){
   position = 0;
 } 
 else if(leftSensor && !rightSensor){
   position = 1; 
 }
 else if(!leftSensor && rightSensor){
   position = -1; 
 }
 else{
   if(lastPosition<0) { //right was on last 
   position = -5; 
   }
   else { // last Position > 0 ==> left was on last 
   position = 5;
   } 
  }

 derivative = (position - lastPosition) / timeStep; 
 PID = (KP * position) + (KD * derivative); 
 
  if(PID>(default_speed)){
   PID = default_speed;
 }

 else if(PID<-(default_speed)){
   PID = -default_speed;
 }
 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0);

 if(lastPosition != position){
   number++;
   if(number==2){
     timeStep = 0;
     number = 0;
   }
  }
 lastPosition = position;
}


void turnLeft(){
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void turnRight(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      return;
    }
  }
}

void stop(){
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  return;
}


void turnInPlaceLeft(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
}

void collectStone(){
  clawServo.write(90);
  armServo.write(0);
 // pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
//pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  while(true){
    if(digitalRead(ARM_SIDES_LIMIT)==HIGH){
      pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      break;
    }
  }
  clawServo.write(180); 
  delay(1000);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3500);
  pwm_start(CLAW_UP, CLOCK_FQ, MAX_SPEED, 0, 0);
  armServo.write(90);
  delay(1000);
  clawServo.write(0);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  delay(3000);
  pwm_start(CLAW_DOWN, CLOCK_FQ, MAX_SPEED, 0, 0);
  return;
}

void dropGauntlet(){
  for (int i = ANGLE_START; i <= ANGLE_FINISH; i = i + 3) {
    gauntletServo.write(i);
    delay(15);
  }
  delay(1000);

  for (int i = ANGLE_START; i >= ANGLE_FINISH; i = i - 3) {
    gauntletServo.write(i);
    delay(15);
  }
  delay(1000);
}

#endif

#ifdef TURN_IN_PLACE
void turnInPlaceLeft();
void turnInPlaceRight();
void stop();

void setup(){
  pinMode(L_MOTOR_FORWARD, OUTPUT);
  pinMode(R_MOTOR_FORWARD, OUTPUT);
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
  pinMode(L_MOTOR_BACKWARD, OUTPUT);
  pinMode(R_MOTOR_BACKWARD, OUTPUT);
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1);
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 1); 
}

void loop(){
  turnInPlaceLeft();
  stop();
  delay(3000);
}

void turnInPlaceLeft(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
}

void turnInPlaceRight(){
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(PHOTO_0) >= THRESHOLD || analogRead(PHOTO_1) >= THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
}

void stop(){
  pwm_start(L_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(R_MOTOR_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(L_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(R_MOTOR_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
}
#endif
 

#ifdef DROP_OFF_STONE
Servo cottonCandy;
Servo clawServo;
Servo armServo;

void setup() {
  Serial.begin(9600);
  cottonCandy.attach(COTTON_CANDY);
  clawServo.attach(CLAW_SERVO);
  armServo.attach(ARM_SERVO);
  cottonCandy.write(70);
  clawServo.write(0);
  //make claw be on right side
  //armServo.write(0);    //robot's right = 0 deg

  //make claw be on left side
  armServo.write(180);
}

void loop() {
  //when claw is on right side of bot

  // armServo.write(0);
  // //right most channel
  // clawServo.write(0);
  // delay(1000);
  // clawServo.write(180);
  // delay(1000);
  // cottonCandy.write(90);
  // armServo.write(80);
  // delay(1000);
  // clawServo.write(0);
  // delay(1000);
  // cottonCandy.write(70);
  // delay(1000);

  // //second right channel
  // armServo.write(0);
  // clawServo.write(0);
  // delay(1000);
  // clawServo.write(180);
  // delay(1000);
  // cottonCandy.write(90);
  // armServo.write(94);
  // delay(1000);
  // clawServo.write(0);
  // delay(1000);
  // cottonCandy.write(70);
  // delay(1000);

  // cottonCandy.write(130);
  // delay(1000);
  // cottonCandy.write(70);
  // delay(1000);


  //when claw is on left side of bot
  armServo.write(180);
  //right most channel
  clawServo.write(0);
  delay(1000);
  clawServo.write(180);
  delay(1000);
  cottonCandy.write(90);
  armServo.write(95);
  delay(1000);
  clawServo.write(0);
  delay(1000);
  cottonCandy.write(70);
  delay(1000);

  //second right channel
  armServo.write(180);
  clawServo.write(0);
  delay(1000);
  clawServo.write(180);
  delay(1000);
  cottonCandy.write(90);
  armServo.write(77);
  delay(1000);
  clawServo.write(0);
  delay(1000);
  cottonCandy.write(70);
  delay(1000);

  cottonCandy.write(150);
  delay(1000);
  cottonCandy.write(70);
  delay(1000);
}
#endif

#ifdef CHECK_TEAM
void setup() {
  pinMode(T_OR_M, INPUT);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(T_OR_M) == HIGH){
    Serial.println("We are THANOS!");
  }
  else{
    Serial.println("We are METHANOS!");
  }
}
#endif

#ifdef CHECK_LIMIT
bool triggered = false;
bool tested = false;

void setup() {
  pinMode(ARM_LEFT, OUTPUT);
  pinMode(ARM_RIGHT, OUTPUT);
  pinMode(ARM_SIDES_LIMIT, INPUT);
  pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 1);
  pwm_start(ARM_RIGHT, CLOCK_FQ, MAX_SPEED, 0 , 1);
}

void loop() {
  triggered = digitalRead(ARM_SIDES_LIMIT) == HIGH;
  if(!triggered && !tested){
    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, MAX_SPEED , 0);
  }
  else{
    tested = true;
    pwm_start(ARM_LEFT, CLOCK_FQ, MAX_SPEED, 0 , 0);
  }

}
#endif