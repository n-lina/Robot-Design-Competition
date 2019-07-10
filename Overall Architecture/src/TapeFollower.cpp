#include "Arduino.h"
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower():
  splitNumber(0), derivative(0), loopCounter(0), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), L_GauntletServo(), R_GauntletServo(), L_lastEncoder(0), L_encoder(0),
  L_distance(0), R_lastEncoder(0), R_encoder(0), R_distance(0)
  {}


void TapeFollower::followTape(){
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
    
    if(state==0){
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)-PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/3)+PID, 0); 
    }

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
    if(state == 0 && loopCounter%10 == 0){
      leftSplit = analogRead(L_SPLIT)>=*p_THRESHOLD;
      rightSplit = analogRead(R_SPLIT)>=*p_THRESHOLD;
      leftTab = analogRead(L_TAB)>=*p_THRESHOLD;
      rightTab = analogRead(R_TAB)>=*p_THRESHOLD;
      if((leftSplit || rightSplit) && (!leftTab && !rightTab)){
        state = 2;
        return;
      }
      else if((leftSplit && leftTab) && (!rightSplit && !rightTab)){
        direction = LEFT; 
        //move some distance with rotary encoder 
        state = 3; 
        return; 
      }
      else if((rightSplit && rightSplit) && (!leftSplit && !leftTab)){
        direction = RIGHT; 
        //move some distance with rotary encoder 
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
  while(true){
    if(analogRead(L_TAPE_FOLLOW)>=*p_THRESHOLD && analogRead(R_TAPE_FOLLOW)>=*p_THRESHOLD){
      return;
    }
  }
}

void TapeFollower::turnRight(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)+(5* *p_KD_WHEEL),0);
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/4)-(5* *p_KD_WHEEL),0);
  while(true){
    if(analogRead(L_TAPE_FOLLOW)>=*p_THRESHOLD && analogRead(R_TAPE_FOLLOW)>=*p_THRESHOLD){
      return;
    }
  }
}

void TapeFollower::goDistance(int distance){ //distance = number of rotary encoder clicks
//if rotary encoder misses clicks, make distance number smaller 
  while(state==6 && L_distance <= distance){
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

    L_encoder = digitalRead(L_ENCODER);
    R_encoder = digitalRead(R_ENCODER);
    if(L_encoder!=L_lastEncoder){
      L_distance++;    
    }
    if(R_encoder!=R_lastEncoder){ //maybe only test one side of rotary encoder for requiring 
    //less frequent sampling 
      R_distance++;
    }

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    L_lastEncoder = L_encoder; 
    R_lastEncoder = R_encoder;
  }
  return;
}

