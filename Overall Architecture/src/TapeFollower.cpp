#include "Arduino.h"
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower():
  splitNumber(0), derivative(0), loopCounter(0), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), L_GauntletServo(), R_GauntletServo()
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
        stop();
        state = 2;
        return;
      }
      else if((leftSplit && leftTab) && (!rightSplit && !rightTab)){
        stop();
        direction = LEFT; 
        //move some distance with rotary encoder 
        state = 3; 
        return; 
      }
      else if((rightSplit && rightSplit) && (!leftSplit && !leftTab)){
        stop();
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
  pwm_start(WHEEL_MOTOR_RIGHT_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  pwm_start(WHEEL_MOTOR_LEFT_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);
  return;
}

