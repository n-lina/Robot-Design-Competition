#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(int& state, int& stoneNumber, int& splitNumber, bool& TEAM, bool& direction_facing, bool& direction):
  _state(state), _TEAM(TEAM), _stoneNumber(stoneNumber), _direction_facing(direction_facing), _direction(LEFT), collisionNumber(0), derivative(0), loopCounter(0), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), leftSensor(0), rightSensor(0), leftSplit(0), rightSplit(0), 
  leftTab(0), rightTab (0), L_GauntletServo(), R_GauntletServo(), lastEncoder(0), encoder(0),
  distance(0), gauntletTapeNumber(0)
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(_state==1){
    leftSensor = analogRead(L_TAPE_FOLLOW)>=THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=THRESHOLD;
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
    PID = (KP_WHEEL * position) + (KD_WHEEL * derivative); 
    
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
    if(loopCounter%10 == 0 && _state ==1){
      leftSensor = analogRead(L_TAPE_FOLLOW) >= THRESHOLD;
      rightSensor = analogRead(R_TAPE_FOLLOW) >= THRESHOLD;
      leftSplit = analogRead(L_SPLIT)>= SPLIT_THRESHOLD;
      rightSplit = analogRead(R_SPLIT)>= SPLIT_THRESHOLD;
      leftTab = analogRead(L_TAB)>= TAB_THRESHOLD;
      rightTab = analogRead(R_TAB)>= TAB_THRESHOLD;

      if((leftSplit || rightSplit) && (leftSensor && rightSensor) && (!leftTab && !rightTab)){
        splitNumber++;
        _state = 3;
        return;
      }
      else if((leftSplit && leftTab) && (leftSensor && rightSensor) && (!rightSplit && !rightTab)){
        _direction = LEFT; 
        //move some distance with rotary encoder 
        stop();
        _state = 4; 
        return; 
      }
      else if((rightSplit && rightTab) && (leftSensor && rightSensor) && (!leftSplit && !leftTab)){
        _direction = RIGHT; 
        //move some distance with rotary encoder 
        stop();
        _state = 4;
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
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+(5* KD_WHEEL),0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-(5* KD_WHEEL),0);
  return;
}

void TapeFollower::turnRight(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+(5* KD_WHEEL),0);
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/sPEED_TUNING)-(5* KD_WHEEL),0);
  return;
}

void TapeFollower::goDistance(int set_distance, bool firstRun, int checkptA, int checkptB){ //distance = number of rotary encoder clicks
//if rotary encoder misses clicks, make distance number smaller 
  while(_state==0 && distance <= set_distance){
    leftSensor = analogRead(L_TAPE_FOLLOW)>= THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>= THRESHOLD;
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
    PID = (KP_WHEEL * position) + (KD_WHEEL * derivative); 
    
    if(_state==0){
      pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
      pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 
    }

    encoder = digitalRead(WHEEL_ENCODER);
    if(encoder!=lastEncoder){ //maybe only test one side of rotary encoder for requiring 
    //less frequent sampling 
      distance++;    
    }

    if(firstRun && _state==0){
      if(distance == checkptA || distance == checkptB){
        if(_TEAM){
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
  if (_direction_facing) _direction_facing = false; 
  else _direction_facing = true;
  return;
}

void TapeFollower::turnInPlaceRight(){
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING), 0);
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, -(MAX_SPEED/SPEED_TUNING, 0); 
  if (_direction_facing) _direction_facing = false; 
  else _direction_facing = true;
  return;
}

void TapeFollower::splitDecide(){
  switch(_splitNumber){
    case 1:
      if(_collisionNumber == 0 || _stoneNumber > 1){
        _state = 7; // go home state 
        return;
      }
      else{
        if(_TEAM){ // may have to travel short distance before turning, 
          turnInPlaceLeft();
          _splitNumber++;
          _state = 6;
          return;
        }
        else{
          turnInPlaceRight();
          _splitNumber++;
          _state = 6;
          return;
        }
      }
    case 2: 
      if(_TEAM){
        turnLeft();
        _state = 0; 
        return; 
      }
      else{
        turnRight();
        _state = 0; 
        return;
      }
    case 3: 
      _state = 7; // go home state 
      return;
  }
}

void TapeFollower::goHome(bool park){ //how to make a timed interrupt for 1 min 30 s, time to go home?
  if(_direction_facing){ 
    if(_direction){
      turnInPlaceLeft(); //facing forward and tabs to the right
    }
    else{
      turnInPlaceRight(); //facing forward and tabs to the left
    }
  }
  while(_state==7){ 
    leftSensor = analogRead(L_TAPE_FOLLOW)>=THRESHOLD;
    rightSensor = analogRead(R_TAPE_FOLLOW)>=THRESHOLD;
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
    PID = (KP_WHEEL * position) + (KD_WHEEL * derivative); 
  
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
    if(loopCounter%10 == 0 && _state ==7){
      if(analogRead(GAUNTLET_TAPE_SENSOR) >= GAUNTLET_TAPE_THRESHOLD){
        gauntletTapeNumber++;
        switch(gauntletTapeNumber){
          case 1: case 4:
            if(_TEAM){
              turnInPlaceLeft(); //make sure main PTs dont catch || gauntlet tapes  
              delay(10); // so that gauntlet tape sensor doesnt catch 
            }
            else{
              turnInPlaceRight();
              delay(10);
            }
            break;
          case 2: case 5: 
            break;
          case 3: case 6:
            stop();    
            L_GauntletServo.write(90);
            R_GauntletServo.write(90); //lower wall
            pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING), 0);
            pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING), 0);
            delay(10);
            stop();
            L_GauntletServo.write(0);
            R_GauntletServo.write(0); // raise wall
            if(_TEAM){
              turnInPlaceLeft();
              delay(10);
            }
            else{
              turnInPlaceRight();
              delay(10);
            }
            if(park){
              state = 8;
            }
            else{
              state = 0;
            }
            return;    
        }
      }
      loopCounter = 0;
    }
  }




