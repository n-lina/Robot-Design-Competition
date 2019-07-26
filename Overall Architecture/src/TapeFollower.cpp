#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_KP_WHEEL(Robot::instance()->KP_WHEEL), 
  my_KD_WHEEL(Robot::instance()->KD_WHEEL), 
  my_THRESHOLD(Robot::instance()->THRESHOLD),
  my_SPLIT_THRESHOLD(Robot::instance()->SPLIT_THRESHOLD), 
  my_TAB_THRESHOLD(Robot::instance()->TAB_THRESHOLD),
  my_TEAM(Robot::instance()->TEAM),
  derivative(0), default_speed(MAX_SPEED/SPEED_TUNING), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), distance(0), pressed(false), homeSplit(false)
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(Robot::instance()->state==FOLLOW_TAPE){
    if(digitalRead(COLLISION)==HIGH){
      stop();
      Robot::instance()->collisionNumber++;
      Robot::instance()->state = AVOID_COLLISION;
      return;
    }

    leftSensor = analogRead(L_TAPE_FOLLOW)>=my_THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=my_THRESHOLD;
    leftSplit = analogRead(L_SPLIT)>= my_SPLIT_THRESHOLD;
    rightSplit = analogRead(R_SPLIT)>= my_SPLIT_THRESHOLD;
    leftTab = analogRead(L_TAB)>= my_TAB_THRESHOLD;
    rightTab = analogRead(R_TAB)>= my_TAB_THRESHOLD;

    if((leftSplit || rightSplit) && (!leftTab && !rightTab) && (leftSensor || rightSensor)){
      stop();
      Robot::instance()->splitNumber++;
      Robot::instance()->state = SPLIT_CHOOSER;
      return;
    }
    else if((leftSplit && leftTab) && (!rightSplit && !rightTab) && (leftSensor || rightSensor)){
      stop();
      Robot::instance()->direction = LEFT; 
      Robot::instance()->state = COLLECT_STONE; 
      return; 
    }
    else if((rightSplit && rightTab) && (!leftSplit && !leftTab) && (leftSensor || rightSensor)){
      stop();
      Robot::instance()->direction = RIGHT; 
      Robot::instance()->state = COLLECT_STONE;
      return;
    }

    timeStep++;

    if (leftSensor  && rightSensor ){
      position = 0;
    } 
    else if(leftSensor  && !rightSensor ){
      position = 1; 
    }
    else if(!leftSensor  && rightSensor ){
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
    PID = (my_KP_WHEEL * position) + (my_KD_WHEEL * derivative); 
    
    if(PID>default_speed){
      PID = default_speed;
    }

    else if(PID<-default_speed){
      PID = -default_speed;
    }

    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0); 

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 

    // if(!pressed && digitalRead(ARM_HOME_SWITCH)==HIGH){
    //   pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
    //   pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
    //   pressed = true;
    // }
  }
return;
}

void TapeFollower::stop(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0);
  return;
}

void TapeFollower::turnLeft(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      return;
    }
  }
}

void TapeFollower::turnLeftSoft(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 550, 0); //turn right
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0); 
  delay(SOFT_TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      return;
    }
  }
}

void TapeFollower::turnRight(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      return;
    }
  }
}
void TapeFollower::turnRightSoft(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0); //turn right
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 550, 0); 
  delay(SOFT_TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      return;
    }
  }
}

void TapeFollower::goStraight(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0);
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0);
  delay(500);
  return; 
}

void TapeFollower::goDistance(int set_distance, bool firstRun, int checkptA, int checkptB){ //distance = number of rotary encoder clicks
//if rotary encoder misses clicks, make distance number smaller 
  while(Robot::instance()->state == GO_DISTANCE && distance <= set_distance){
    leftSensor = analogRead(L_TAPE_FOLLOW)>= my_THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>= my_THRESHOLD;
    timeStep++;

    if (leftSensor  && rightSensor ){
      position = 0;
    } 
    else if(leftSensor  && !rightSensor ){
      position = 1; 
    }  
    else if(!leftSensor  && rightSensor ){
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
    PID = (my_KP_WHEEL * position) + (my_KD_WHEEL * derivative); 
    
    if(Robot::instance()->state==GO_DISTANCE){
      pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
      pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 
    }

    encoder = digitalRead(L_WHEEL_ENCODER);
    if(encoder!=lastEncoder){ //maybe only test one side of rotary encoder for requiring 
    //less frequent sampling 
      distance++;    
    }

    if(firstRun && Robot::instance()->state==GO_DISTANCE){
      if(distance == checkptA || distance == checkptB){
        if(Robot::instance()->TEAM){
          turnLeft();
          }
        else{
          turnRight();
        }
      }
    }

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    lastEncoder = encoder; 
  }
  return;
}

void TapeFollower::turnInPlaceLeft(){
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  pwm_start(LEFT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      stop();
      Robot::instance()->state=FOLLOW_TAPE;
      return;
    }
  }
  return;
}

void TapeFollower::turnInPlaceRight(){
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  pwm_start(RIGHT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, MAX_SPEED, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      stop();
      Robot::instance()->state=FOLLOW_TAPE;
      return;
    }
  }
  return;
}

void TapeFollower::splitDecide(){
  if(my_TEAM){
    switch(Robot::instance()->splitNumber){
      case 1:
        turnLeft();
        break;
      case 2:
        turnRightSoft();
        break;
    }
  }
  else{
    switch(Robot::instance()->splitNumber){
      case 1:
        turnRight(); //gauntlet split
        break;
      case 2:
        turnLeftSoft(); //path split 
        break;
    }
  }
}

//only call between 2nd split and 1st tabs 
void TapeFollower::goHome(){ //how to make a timed interrupt for 1 min 30 s, time to go home?
  if(Robot::instance()->direction_facing){ 
    if(Robot::instance()->direction){
      turnInPlaceLeft(); //facing forward and tabs to the right
    }
    else{
      turnInPlaceRight(); //facing forward and tabs to the left
    }
  }
  while(Robot::instance()->state==GO_HOME){ 
    leftSensor = analogRead(L_TAPE_FOLLOW)>=my_THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=my_THRESHOLD;
    timeStep++;

    if (leftSensor  && rightSensor ){
      position = 0;
    } 
    else if(leftSensor  && !rightSensor ){
      position = 1; 
    }
    else if(!leftSensor  && rightSensor ){
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
    PID = (my_KP_WHEEL * position) + (my_KD_WHEEL * derivative); 
  
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0); 
    
    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 

    if((leftSplit || rightSplit) && (leftSensor || rightSensor) && (!leftTab && !rightTab)){
      if(homeSplit){
        if(Robot::instance()->TEAM){ //thanos
          turnRight();
          delay(2000);
          //TODO
        }
        else{
          turnLeft();
          delay(2000);
          //TODO
        }  
        Robot::instance()->L_GauntletServo.write(180); //lower gauntlet 
        goStraight();
        delay(20);
        stop();
        Robot::instance()->state = PARK;
        return;
      }
    homeSplit=true;
    }
  }
return;
}

void TapeFollower::park(){
  pwm_stop(LEFT_FORWARD_WHEEL_MOTOR);
  pwm_stop(RIGHT_FORWARD_WHEEL_MOTOR);
  pwm_stop(LEFT_BACKWARD_WHEEL_MOTOR);
  pwm_stop(RIGHT_BACKWARD_WHEEL_MOTOR);
}

void TapeFollower::avoidCollision(){
  if(Robot::instance()->direction){
    turnInPlaceLeft(); //facing forward and tabs to the right
  }
  else{
    turnInPlaceRight(); //facing forward and tabs to the left
  }
  //TODO : use rotary encoders to kno how much to increment stoneNumber
}




