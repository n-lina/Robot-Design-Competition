#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_TEAM(Robot::instance()->TEAM),
  my_DECIDE_THRESHOLD(Robot::instance()->DECIDE_THRESHOLD),
  my_ALIGN_THRESHOLD(Robot::instance()->ALIGN_THRESHOLD),
  my_THRESHOLD(Robot::instance()->THRESHOLD),
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
  pressed(false), 
  my_path()
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(Robot::instance()->state==FOLLOW_TAPE){
    
    // if(digitalRead(COLLISION)==HIGH){ 
    //   stop();
    //   Robot::instance()->collisionNumber++;
    //   Robot::instance()->state = AVOID_COLLISION;
    //   return;
    // }

    if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
      digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      digitalWrite(DEMULTIPLEX_ARM_H, LOW);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      pressed = true;
    }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>=my_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>=my_THRESHOLD;
    leftDecide = analogRead(L_DECIDE)>= my_DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE)>=my_DECIDE_THRESHOLD;

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
    
    digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
    digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
    pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0);
    pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0); 

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
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0); 
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0); 
  return;
}

void TapeFollower::turnLeft(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD || analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD){
      return;
    }
  }
}

void TapeFollower::turnRight(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD || analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD){
      return;
    }
  }
}

void TapeFollower::goDistance(int loopNumber){ 
  while(loopCounter < loopNumber){
    if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
      digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      digitalWrite(DEMULTIPLEX_ARM_H, LOW);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      pressed = true;
    }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>= my_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>= my_THRESHOLD;
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
    
    digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
    digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
    pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
    pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 

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
  Robot::instance()->state = FOLLOW_TAPE; 
  return;
}

void TapeFollower::turnInPlaceLeft(){
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD || analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD){
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
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 280, 0); 
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 280, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD || analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD){
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
          if(rightDecide || analogRead(R_DECIDE) > my_DECIDE_THRESHOLD){
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
          if(leftDecide || analogRead(L_DECIDE) > my_DECIDE_THRESHOLD){
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
      digitalWrite(LEFT_WHEEL, HIGH);
      digitalWrite(RIGHT_WHEEL, HIGH);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 600, 0);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 800, 0);
      delay(30);
      my_path.pop();
      default_speed = MAX_SPEED; 
      Robot::instance()->state = FOLLOW_TAPE;
      return;
    }
    else if(my_path.top() == SPLIT && Robot::instance()->junctionNumber < TOP_PATH_SIZE){ 
      // first time encountering path split facing backwards    
      digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
      digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 400, 0);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 400, 0);
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
    leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD;
    leftDecide = analogRead(L_DECIDE) >= my_DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE) >= my_DECIDE_THRESHOLD;
    
    if(leftDecide|| rightDecide) {
      if(Robot::instance()->TEAM){ //thanos
        stop();
        turnLeft();
        digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
        digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
        pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
        pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
        delay(500);
        turnInPlaceRight();
      }
      else{
        stop();
        turnRight();
        digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
        digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
        pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
        pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
        delay(500);
        turnInPlaceLeft();
      }  
      digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH); // going straight towards to gauntlet
      digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
      delay(500);
      dropGauntlet();
      digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH); //moving a bit to push the stones in place
      digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0);
      delay(20);
      stop();
      Robot::instance()->state = PARK;
      return;
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

    digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
    digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
    pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0);
    pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0); 
    
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
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_stop(LEFT_WHEEL);
  pwm_stop(RIGHT_WHEEL);
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_stop(LEFT_WHEEL);
  pwm_stop(RIGHT_WHEEL);
  digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
  pwm_stop(ARM_MOTOR_H);
  digitalWrite(DEMULTIPLEX_ARM_H, LOW);
  pwm_stop(ARM_MOTOR_H);
  digitalWrite(DEMULTIPLEX_ARM_V, HIGH);
  pwm_stop(ARM_MOTOR_V);
  digitalWrite(DEMULTIPLEX_ARM_V, LOW);
  pwm_stop(ARM_MOTOR_V);
  Robot::instance()->state = PARK;
  return;
}

void TapeFollower::avoidCollision(){ //TODO
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW); //backing up
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
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
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 170, 0); //very slow
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::alignPillarLeft(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 170, 0); //very slow
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 250, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::alignPillarRight(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 250, 0); //very slow
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 170, 0);
  while(true){
    getPosition();
    if(analogRead(L_ALIGN) >= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
    else if(analogRead(R_ALIGN)>= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      Robot::instance()->state = COLLECT_STONE; 
      return;
    }
  }
  return;
}

void TapeFollower::getPosition(){ // to return to tape after aligning with pillar 
  leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD;
  rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD;
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







