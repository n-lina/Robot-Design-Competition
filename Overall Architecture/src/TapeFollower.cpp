#include "Arduino.h"
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower():
  splitNumber(0), derivative(0), loopCounter(0), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), L_GauntletServo(), R_GauntletServo(), lastEncoder(0), encoder(0),
  distance(0)
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(state==0){
    leftSensor = analogRead(L_TAPE_FOLLOW)>=*p_THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=*p_THRESHOLD;
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
    PID = (*p_KP_WHEEL * position) + (*p_KD_WHEEL * derivative); 
    
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)-PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)+PID, 0); 

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
    if(loopCounter%10 == 0){
      leftSensor = analogRead(L_TAPE_FOLLOW) >= *p_THRESHOLD;
      rightSensor = analogRead(R_TAPE_FOLLOW) >= *p_THRESHOLD;
      leftSplit = analogRead(L_SPLIT)>=*p_SPLIT_THRESHOLD;
      rightSplit = analogRead(R_SPLIT)>=*p_SPLIT_THRESHOLD;
      leftTab = analogRead(L_TAB)>=*p_TAB_THRESHOLD;
      rightTab = analogRead(R_TAB)>=*p_TAB_THRESHOLD;

      if((leftSplit || rightSplit) && (leftSensor && rightSensor) && (!leftTab && !rightTab)){
        splitNumber++;
        state = 2;
        return;
      }
      else if((leftSplit && leftTab) && (leftSensor && rightSensor) && (!rightSplit && !rightTab)){
        direction = LEFT; 
        //move some distance with rotary encoder 
        stop();
        state = 3; 
        return; 
      }
      else if((rightSplit && rightTab) && (leftSensor && rightSensor) && (!leftSplit && !leftTab)){
        direction = RIGHT; 
        //move some distance with rotary encoder 
        stop();
        state = 3;
         return;
      }
      loopCounter = 0;
    }
  }
}

void TapeFollower::stop(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0);
  return;
}

void TapeFollower::turnLeft(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+(5* *p_KD_WHEEL),0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-(5* *p_KD_WHEEL),0);
  return;
}

void TapeFollower::turnRight(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+(5* *p_KD_WHEEL),0);
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-(5* *p_KD_WHEEL),0);
  return;
}

void TapeFollower::goDistance(int set_distance){ //distance = number of rotary encoder clicks
//if rotary encoder misses clicks, make distance number smaller 
  while(state==6 && distance <= set_distance){
    leftSensor = analogRead(L_TAPE_FOLLOW)>=*p_THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=*p_THRESHOLD;
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
    PID = (*p_KP_WHEEL * position) + (*p_KD_WHEEL * derivative); 
    
    if(state==6){
      pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)-PID, 0);
      pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)+PID, 0); 
    }

    encoder = digitalRead(WHEEL_ENCODER);
    if(encoder!=lastEncoder){ //maybe only test one side of rotary encoder for requiring 
    //less frequent sampling 
      distance++;    
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
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, -(MAX_SPEED/3), 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3), 0); 
  if (direction_facing) direction_facing = false; 
  else direction_facing = true;
  return;
}

void TapeFollower::turnInPlaceRight(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3), 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, -(MAX_SPEED/3), 0); 
  if (direction_facing) direction_facing = false; 
  else direction_facing = true;
  return;
}

void TapeFollower::splitDecide(){
  switch(splitNumber){
    case 1:
      if(TEAM){
        turnLeft();
      }
      else{
        turnRight();
      }
    case 2:
      if(TEAM){
        turnLeft();
        state = 6;
        return;
      }
      else{
        turnRight();
        state = 6;
        return;
      }
    case 3:
      if(collisionNumber == 0){
        state = 7; // go home state 
        return;
      }
      else{
        if(TEAM){ // may have to travel short distance before turning, 
          turnInPlaceLeft();
          splitNumber++;
          state = 6;
          return;
        }
        else{
          turnInPlaceRight();
          splitNumber++;
          state = 6;
          return;
        }
      }
    case 4: 
      state = 7; // go home state 
      return;
  }
}




