#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_TEAM(Robot::instance()->TEAM),
  derivative(0), 
  default_speed(MAX_SPEED), 
  timeStep(0), 
  position(0), 
  lastPosition(0), 
  PID(0), 
  number(0), 
  debounce(0), 
  leftTapeFollow(0), 
  rightTapeFollow(0),
  leftDecide(0), 
  rightDecide(0), 
  leftAlign(0), 
  rightAlign(0), 
  loopCounter(0),
  atGauntlet(false),
  pressed(false), 
  my_path()
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(Robot::instance()->state==FOLLOW_TAPE){
    
    if(digitalRead(COLLISION)==HIGH){ 
      stop();
      Robot::instance()->collisionNumber++;
      Robot::instance()->state = AVOID_COLLISION;
      return;
    }

    if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
      pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
      pressed = true;
    }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>=_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>=_THRESHOLD;
    leftDecide = analogRead(L_DECIDE)>= _DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE)>=_DECIDE_THRESHOLD;
    leftDecide=true;
    if((leftDecide || rightDecide) && (debounce > DEBOUNCE)){
      stop();
      debounce = 0;
      Robot::instance()->state = SPLIT_CHOOSER; 
      return;
    }

    timeStep++;

    if (leftTapeFollow  && rightTapeFollow){
      position = 0;
    } 
    else if(leftTapeFollow  && !rightTapeFollow ){
      position = 1; 
    }
    else if(!leftTapeFollow  && rightTapeFollow){
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
    
    if(PID>default_speed){
      PID = default_speed;
    }

    else if(PID<-default_speed){
      PID = -default_speed;
    }
    
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
    debounce++;
  }
return;
}

void TapeFollower::stop(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  return;
}

void TapeFollower::turnLeft(){

  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

void TapeFollower::turnRight(){
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

void TapeFollower::goDistance(int loopNumber){ 
  while(loopCounter < loopNumber){
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
    
    pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
    pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 

    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
    loopCounter++;
    Serial.println(String(loopCounter));
  }
  Robot::instance()->state = FOLLOW_TAPE; 
  return;
}

void TapeFollower::turnInPlaceLeft(){
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      debounce = 500;
      Robot::instance()->state = FOLLOW_TAPE; 
      return;
    }
  }
  return;
}

void TapeFollower::turnInPlaceRight(){
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  pwm_start(RIGHT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      debounce = 500;
      Robot::instance()->state = FOLLOW_TAPE; 
      return;
    }
  }
  return;
}

void TapeFollower::splitDecide(){ 
  if(Robot::instance()->direction_facing){
    if(THANOS){
      switch(Robot::instance()->junctionNumber){
        case 0: 
          stop();
          my_path.push(SPLIT);
          turnLeft();
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 1: case 4:
          if(rightDecide || analogRead(R_DECIDE) > _DECIDE_THRESHOLD){
            turnRight();
          }
          my_path.push(SPLIT);
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 2: 
          my_path.push(TAB);
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 3: 
          stop();
          my_path.push(TAB);
          delay(1000);
          alignPillar();
          Robot::instance()->state = COLLECT_STONE;
          break;  
        case 5: 
          stop();
          my_path.push(TAB);
          delay(1000);
          alignPillarRight();
          Robot::instance()->state = COLLECT_STONE;
          break;  
      }
    Serial.println("Junction Number: " + String(Robot::instance()->junctionNumber));
    Serial.println("Top: " + String(my_path.top()));
    Serial.println("Size: " + String(my_path.size()));
    Serial.println("State: " + String(Robot::instance()->state));
    }
    else{
      switch(Robot::instance()->junctionNumber){
        case 0: 
          stop();
          my_path.push(SPLIT);
          turnRight();
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 1: case 4: 
          if(leftDecide || analogRead(L_DECIDE) > _DECIDE_THRESHOLD){
            turnLeft();
          }
          my_path.push(SPLIT);
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 2: 
          my_path.push(TAB);
          Robot::instance()->state = FOLLOW_TAPE;
          break;
        case 3: //first stone on bottom path 
          stop();
          my_path.push(TAB);
          delay(1000);
          alignPillar();
          Robot::instance()->state = COLLECT_STONE;
          break;   
        case 5:
          stop();
          my_path.push(TAB);
          delay(1000);
          alignPillarLeft();
          Robot::instance()->state = COLLECT_STONE;
          break;   
      }
    }
  Robot::instance()->junctionNumber++;
  return;
  }
  else{ //facing backwards
    if(my_path.top() == TAB && my_path.size() == TOP_PATH_SIZE){
      my_path.pop();
      default_speed = 700;
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
    else if(my_path.top() == TAB && my_path.size() < TOP_PATH_SIZE){
      pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 600, 0);
      pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 800, 0);
      delay(30);
      my_path.pop();
      default_speed = MAX_SPEED; 
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
    else if(my_path.top() == SPLIT && Robot::instance()->junctionNumber < TOP_PATH_SIZE){ 
      // first time encountering path split facing backwards    
      pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
      pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 400, 0);
      delay(700);
      turnInPlaceRight();
      my_path.pop();
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
    else if(my_path.size() == SPLIT && Robot::instance()->junctionNumber > TOP_PATH_SIZE){
      // second time encountering path split facing backwards 
      my_path.pop();
      Robot::instance()->state = GO_HOME;
      return;
    }
  }
}

//only call between 2nd split and 1st tabs 
void TapeFollower::goHome(){ 
  // if(Robot::instance()->direction_facing){ 
  //   if(Robot::instance()->direction){
  //     turnInPlaceLeft(); //facing forward and tabs to the right
  //   }
  //   else{
  //     turnInPlaceRight(); //facing forward and tabs to the left
  //   }
  // }
  while(Robot::instance()->state==GO_HOME){ 
    leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= _THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= _THRESHOLD;
    leftDecide = analogRead(L_DECIDE) >= _DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE) >= _DECIDE_THRESHOLD;
    
    if(leftDecide || rightDecide) {
      if(!atGauntlet){
        if(Robot::instance()->TEAM){ //thanos
          stop();
          turnLeft();
          pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
          pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
          delay(500);
          turnInPlaceRight();
        }
        else{
          stop();
          turnRight();
          pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
          pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
          delay(500);
          turnInPlaceLeft();
        }  
      atGauntlet = true;
      }
      else{
        delay(300);
        stop();
        Robot::instance()->state = PARK;
        return;
      }
    }
    timeStep++;

    if (leftTapeFollow  && rightTapeFollow){
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

    pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0);
    pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0); 
    
    if(lastPosition != position){
      number++;
      if(number==2){
        timeStep = 0;
        number = 0;
      } 
    }
    lastPosition = position; 
  }
return;
}

void TapeFollower::park(){
  pwm_stop(LEFT_WHEEL_FORWARD);
  pwm_stop(RIGHT_WHEEL_FORWARD);
  pwm_stop(LEFT_WHEEL_BACKWARD);
  pwm_stop(RIGHT_WHEEL_BACKWARD);
  pwm_stop(ARM_MOTOR_RIGHT);
  pwm_stop(ARM_MOTOR_LEFT);
  pwm_stop(ARM_MOTOR_UP);
  pwm_stop(ARM_MOTOR_DOWN);
  Robot::instance()->state = PARK;
  return;
}

void TapeFollower::avoidCollision(){ //TODO
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
  pwm_start(LEFT_WHEEL_BACKWARD, CLOCK_FQ, MAX_SPEED, 300, 0);
  delay(500);
  if(Robot::instance()->direction_facing){
    if(Robot::instance()->direction){
      turnInPlaceLeft(); //facing forward and tabs to the right
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
    else{
      turnInPlaceRight(); //facing forward and tabs to the left
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
  }
  else{
    return; //TODO
  }
}

void TapeFollower::alignPillar(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0); //very slow
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::alignPillarLeft(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0); //very slow
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 250, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::alignPillarRight(){
  pwm_start(LEFT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 250, 0); //very slow
  pwm_start(RIGHT_WHEEL_FORWARD, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= _ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::getPosition(){ // to return to tape after aligning with pillar 
  leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= _THRESHOLD;
  rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= _THRESHOLD;
    if (leftTapeFollow  && rightTapeFollow ){
      position = 0;
    } 
    else if(leftTapeFollow  && !rightTapeFollow ){
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

void TapeFollower::dropGauntlet(){
  for(volatile int i=0; i<10; i++){
    Robot::instance()->L_GauntletServo.write(150);
    delay(1000);
    Robot::instance()->L_GauntletServo.write(140);
    delay(1000);
  }
  Robot::instance()->state = PARK;
}







