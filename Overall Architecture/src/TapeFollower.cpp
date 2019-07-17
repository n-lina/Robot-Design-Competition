#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_KP_WHEEL(Robot::instance()->KP_WHEEL), 
  my_KD_WHEEL(Robot::instance()->KD_WHEEL), 
  my_THRESHOLD(Robot::instance()->THRESHOLD),
  my_SPLIT_THRESHOLD(Robot::instance()->SPLIT_THRESHOLD), 
  my_TAB_THRESHOLD(Robot::instance()->TAB_THRESHOLD),
  my_ALIGN_TAB_THRESHOLD(Robot::instance()->ALIGN_TAB_THRESHOLD),
  my_EDGE_THRESHOLD(Robot::instance()->EDGE_THRESHOLD),
  my_COLLISION_THRESHOLD(Robot::instance()->COLLISION_THRESHOLD), 
  maxDrivingSpeed((MAX_SPEED/SPEED_TUNING)+(5*KP_WHEEL)+(10*KD_WHEEL)),
  minDrivingSpeed((MAX_SPEED/SPEED_TUNING)-(5*KP_WHEEL)-(10*KD_WHEEL)),
  derivative(0), loopCounter(0), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), distance(0)
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(Robot::instance()->state==FOLLOW_TAPE){
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
    
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
    if(loopCounter%10 == 0 && Robot::instance()->state ==FOLLOW_TAPE){
      leftSensor = analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD;
      rightSensor = analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD;
      leftSplit = analogRead(L_SPLIT)>= my_SPLIT_THRESHOLD;
      rightSplit = analogRead(R_SPLIT)>= my_SPLIT_THRESHOLD;
      leftTab = analogRead(L_TAB)>= my_TAB_THRESHOLD;
      rightTab = analogRead(R_TAB)>= my_TAB_THRESHOLD;

      if((leftSplit || rightSplit) && (leftSensor && rightSensor) && (!leftTab && !rightTab)){
        Robot::instance()->splitNumber++;
        Robot::instance()->state = SPLIT_CHOOSER;
        return;
      }
      else if((leftSplit && leftTab) && (leftSensor && rightSensor) && (!rightSplit && !rightTab)){
        Robot::instance()->direction = LEFT; 
        //move some distance with rotary encoder 
        stop();
        Robot::instance()->state = COLLECT_STONE; 
        return; 
      }
      else if((rightSplit && rightTab) && (leftSensor && rightSensor) && (!leftSplit && !leftTab)){
        Robot::instance()->direction = RIGHT; 
        //move some distance with rotary encoder 
        stop();
        Robot::instance()->state = COLLECT_STONE;
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
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, maxDrivingSpeed,0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, minDrivingSpeed,0);
  return;
}

void TapeFollower::turnRight(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, maxDrivingSpeed,0);
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, minDrivingSpeed,0);
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

    encoder = digitalRead(WHEEL_ENCODER);
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
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, -(MAX_SPEED/SPEED_TUNING), 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING), 0); 
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  return;
}

void TapeFollower::turnInPlaceRight(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING), 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, -(MAX_SPEED/SPEED_TUNING), 0); 
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  return;
}

void TapeFollower::splitDecide(){
  switch(Robot::instance()->splitNumber){
    case 1:
      if(Robot::instance()->collisionNumber == 0 || Robot::instance()->stoneNumber > 1){
        Robot::instance()->state = GO_HOME; // go home state 
        return;
      }
      else{
        if(Robot::instance()->TEAM){ // may have to travel short distance before turning, 
          turnInPlaceLeft();
          Robot::instance()->splitNumber++;
          Robot::instance()->state = GO_HOME;
          return;
        }
        else{
          turnInPlaceRight();
          Robot::instance()->splitNumber++;
          Robot::instance()->state = GO_HOME;
          return;
        }
      }
    case 2: 
      if(Robot::instance()->TEAM){
        turnLeft();
        Robot::instance()->state = GO_DISTANCE; 
        return; 
      }
      else{
        turnRight();
        Robot::instance()->state = GO_DISTANCE; 
        return;
      }
    case 3: 
      Robot::instance()->state = GO_HOME; // go home state 
      return;
  }
}

void TapeFollower::goHome(bool park){ //how to make a timed interrupt for 1 min 30 s, time to go home?
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
  
    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 
    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
    if(loopCounter%10 == 0 && Robot::instance()->state == GO_HOME){
      if(analogRead(leftSplit) >= SPLIT_THRESHOLD){
        turnRight();
        delay(2000);
        //TODO
      }
      else if(analogRead(rightSplit) >= SPLIT_THRESHOLD){
        turnLeft();
        delay(2000);
        //TODO
      }
      if(park){
        Robot::instance()->state == PARK;
      }
      loopCounter = 0;
    }
  }
}




