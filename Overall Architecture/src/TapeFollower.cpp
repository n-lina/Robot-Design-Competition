#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_TEAM(Robot::instance()->TEAM),
  derivative(0), 
  default_speed(MAX_SPEED/SPEED_TUNING), 
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
  my_path(),
  topPath{SPLIT, SPLIT, TAB, TAB}, 
  bottomPath{SPLIT, SPLIT, TAB, TAB, TAB, TAB}
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
      digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      digitalWrite(DEMULTIPLEX_ARM_H, LOW);
      pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
      pressed = true;
    }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>=_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>=_THRESHOLD;
    leftDecide = analogRead(L_DECIDE)>= _DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE)>=_DECIDE_THRESHOLD;

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
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

// void TapeFollower::turnLeftSoft(){
//   pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 550, 0); //turn right
//   pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0); 
//   delay(SOFT_TURN_DELAY_TIME);
//   while(true){
//     if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
//       return;
//     }
//   }
// }

void TapeFollower::turnRight(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      return;
    }
  }
}

void TapeFollower::goDistance(int loopNumber){ 
  while(loopCounter < loopNumber){
    // if(!pressed && digitalRead(ARM_SIDES_LIMIT)==HIGH){
    //   digitalWrite(DEMULTIPLEX_ARM_H, HIGH);
    //   pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
    //   digitalWrite(DEMULTIPLEX_ARM_H, LOW);
    //   pwm_start(ARM_MOTOR_H, CLOCK_FQ, MAX_SPEED, 0, 0);
    //   pressed = true;
    // }

    // leftTapeFollow = analogRead(L_TAPE_FOLLOW)>= _THRESHOLD;
    // rightTapeFollow = analogRead(R_TAPE_FOLLOW)>= _THRESHOLD;
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
    
    // digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
    // digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
    // pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
    // pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 

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
  Serial.println("done goDistance");
  Robot::instance()->state = FOLLOW_TAPE; 
  return;
}

void TapeFollower::turnInPlaceLeft(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0); 
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      delay(50);
      turnRight();
      return;
    }
  }
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  return;
}


void TapeFollower::turnInPlaceRight(){
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0); 
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0); 
  delay(TURN_IN_PLACE_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _THRESHOLD){
      stop();
      delay(50);
      turnLeft();
      return;
    }
  }
  if (Robot::instance()->direction_facing) Robot::instance()->direction_facing = false; 
  else Robot::instance()->direction_facing = true;
  return;
}

void TapeFollower::splitDecide(){ //TODO
  if(Robot::instance()->direction_facing){
    my_path.push(topPath[Robot::instance()->junctionNumber]);
    if(THANOS){
      switch(Robot::instance()->junctionNumber){
        case 0: 
          stop();
          turnLeft();
          return;
        case 1: 
          if(leftDecide){
            turnRight();
          }
          return;
        case 2: 
          return;
        case 3: 
          digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
          digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
          pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 450, 0);
          pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 450, 0);
          delay(1500);
          turnInPlaceLeft();
          return;   
        case 4: //first stone on bottom path 
          my_path.push(TAB);
          alignPillar();
          Robot::instance()->state = COLLECT_STONE; 
          return;  
      }
    }
    else{
      switch(Robot::instance()->junctionNumber){
        case 0: 
          stop();
          turnRight();
          return;
        case 1: 
          if(leftDecide){
            turnLeft();
          }
          return;
        case 2: 
          return;
        case 3: 
          digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
          digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
          pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 450, 0);
          pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 450, 0);
          delay(1500);
          turnInPlaceRight();
          return;
        case 4: //first stone on bottom path 
          my_path.push(TAB);
          alignPillar();
          Robot::instance()->state = COLLECT_STONE; 
          return;          
      }
    }
  Robot::instance()->junctionNumber++;
  }
  else{ //facing backwards
    if(my_path.top() == TAB){
      alignPillar();
      my_path.pop();
      Robot::instance()->state = COLLECT_STONE;
      return;
    }
    else{
      Robot::instance()->state = GO_HOME; //going home at path split
      return;
    }
  }
}

//only call between 2nd split and 1st tabs 
void TapeFollower::goHome(){ //TODO 
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
      digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
      digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 300, 0);
      delay(500);
      dropGauntlet();
      digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
      digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
      pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0);
      pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 500, 0);
      delay(20);
      stop();
      Robot::instance()->state = PARK;
      return;
      debounce=0;
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
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
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
  stop();
  digitalWrite(DEMULTIPLEX_L_WHEEL, HIGH);
  digitalWrite(DEMULTIPLEX_R_WHEEL, HIGH);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 100, 0); //very slow
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 100, 0);
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

void TapeFollower::turnRightAfterPillar(){ //when there is a right turn coming out of a pillar 
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 200, 0); 
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= _ALIGN_THRESHOLD || analogRead(R_TAPE_FOLLOW) >= _ALIGN_THRESHOLD){
      return;
    }
  }
}

void TapeFollower::alignPillarSixTHANOS(){
  digitalWrite(DEMULTIPLEX_L_WHEEL, LOW);
  digitalWrite(DEMULTIPLEX_R_WHEEL, LOW);
  pwm_start(LEFT_WHEEL, CLOCK_FQ, MAX_SPEED, 350, 0); //very slow
  pwm_start(RIGHT_WHEEL, CLOCK_FQ, MAX_SPEED, 160, 0);
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
  for (int i = ANGLE_START; i <= ANGLE_FINISH; i = i + 3) {
    Robot::instance()->L_GauntletServo.write(i);
    delay(15);
  }
  delay(1000);

  for (int i = ANGLE_START; i >= ANGLE_FINISH; i = i - 3) {
    Robot::instance()->L_GauntletServo.write(i);
    delay(15);
  }
  delay(1000);
}





