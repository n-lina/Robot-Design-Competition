#include <Arduino.h>
#include "Constants.h"
#include "TapeFollower.h"

TapeFollower::TapeFollower(Robot const* robot):
  my_KP_WHEEL(Robot::instance()->KP_WHEEL), 
  my_KD_WHEEL(Robot::instance()->KD_WHEEL), 
  my_THRESHOLD(Robot::instance()->THRESHOLD),
  my_DECIDE_THRESHOLD(Robot::instance()->DECIDE_THRESHOLD), 
  my_ALIGN_THRESHOLD(Robot::instance()->ALIGN_THRESHOLD),
  my_TEAM(Robot::instance()->TEAM),
  derivative(0), default_speed(MAX_SPEED/SPEED_TUNING), timeStep(0), position(0), lastPosition(0), 
  PID(0), number(0), debounce(0), leftTapeFollow(0), rightTapeFollow(0),
  leftDecide(0), rightDecide(0), leftAlign(0), rightAlign(0), distance(0),
  pressed(false), homeSplit(false)
  {}


void TapeFollower::followTape(){ //add encoder polling
  while(Robot::instance()->state==FOLLOW_TAPE){
    if(digitalRead(COLLISION)==HIGH){
      stop();
      Robot::instance()->collisionNumber++;
      Robot::instance()->state = AVOID_COLLISION;
      return;
    }

    // if(!pressed && multi(1,0,0)==HIGH){
    // pwm_start(ARM_MOTOR_LEFT, CLOCK_FQ, MAX_SPEED, 0, 0);
    // pwm_start(ARM_MOTOR_RIGHT, CLOCK_FQ, MAX_SPEED, 0, 0);
    // pressed = true;
    // }

    leftTapeFollow = analogRead(L_TAPE_FOLLOW)>=my_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW)>=my_THRESHOLD;
    leftDecide = analogRead(L_DECIDE)>= my_DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE)>= my_DECIDE_THRESHOLD;

    if((leftDecide || rightDecide) && (leftTapeFollow || rightTapeFollow) && (debounce > DEBOUNCE)){
      stop();
      debounce = 0;
      Robot::instance()->splitNumber++;
      Robot::instance()->state = SPLIT_CHOOSER; //TODO: mental memory map 
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
    PID = (my_KP_WHEEL * position) + (my_KD_WHEEL * derivative); 
    
    if(PID>default_speed){
      PID = default_speed;
    }

    else if(PID<-default_speed){
      PID = -default_speed;
    }

    pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, default_speed+PID, 0);
    pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, default_speed-PID, 0); 

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
  pwm_start(LEFT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0);  
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); 
  return;
}

void TapeFollower::turnLeft(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); 
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
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
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 0, 0); //turn right
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 700, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
      return;
    }
  }
}

// void TapeFollower::turnRightSoft(){
//   pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0); //turn right
//   pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 550, 0); 
//   delay(SOFT_TURN_DELAY_TIME);
//   while(true){
//     if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
//       return;
//     }
//   }
// }

// void TapeFollower::goStraight(){
//   pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0);
//   pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0);
//   delay(500);
//   return; 
// }

// void TapeFollower::goDistance(int set_distance, bool firstRun, int checkptA, int checkptB){ //distance = number of rotary encoder clicks
// //if rotary encoder misses clicks, make distance number smaller 
//   while(Robot::instance()->state == GO_DISTANCE && distance <= set_distance){
//     leftSensor = analogRead(L_TAPE_FOLLOW)>= my_THRESHOLD;
//     rightSensor = analogRead(R_TAPE_FOLLOW)>= my_THRESHOLD;
//     timeStep++;

//     if (leftSensor  && rightSensor ){
//       position = 0;
//     } 
//     else if(leftSensor  && !rightSensor ){
//       position = 1; 
//     }  
//     else if(!leftSensor  && rightSensor ){
//       position = -1; 
//     }
//     else{
//       if(lastPosition<0) { //right was on last 
//       position = -5; 
//       }
//       else { // last Position > 0 ==> left was on last 
//       position = 5;
//       }   
//     } 
//     derivative = (position - lastPosition) / timeStep; 
//     PID = (my_KP_WHEEL * position) + (my_KD_WHEEL * derivative); 
    
//     if(Robot::instance()->state==GO_DISTANCE){
//       pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)-PID, 0);
//       pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, (MAX_SPEED/SPEED_TUNING)+PID, 0); 
//     }

//     encoder = digitalRead(L_WHEEL_ENCODER);
//     if(encoder!=lastEncoder){ //maybe only test one side of rotary encoder for requiring 
//     //less frequent sampling 
//       distance++;    
//     }

//     if(firstRun && Robot::instance()->state==GO_DISTANCE){
//       if(distance == checkptA || distance == checkptB){
//         if(Robot::instance()->TEAM){
//           turnLeft();
//           }
//         else{
//           turnRight();
//         }
//       }
//     }

//     if(lastPosition != position){
//       number++;
//       if(number==2){
//         timeStep = 0;
//         number = 0;
//       } 
//     }
//     lastPosition = position; 
//     lastEncoder = encoder; 
//   }
//   return;
// }

void TapeFollower::turnInPlaceLeft(){
  pwm_start(LEFT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
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
  pwm_start(RIGHT_BACKWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0); 
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 900, 0); 
  delay(TURN_DELAY_TIME);
  while(true){
    if(analogRead(L_TAPE_FOLLOW) >= THRESHOLD || analogRead(R_TAPE_FOLLOW) >= THRESHOLD){
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
  if(my_TEAM){ // thanos
    switch(Robot::instance()->splitNumber){
      case 1: case 2: case 7: // when it detects a junction in followTape(), it already stops
        turnLeft(); 
        Robot::instance()->state = FOLLOW_TAPE;
        return;
      case 3: case 4: case 5: case 6: case 8: 
        alignPillar();
        //Robot::instance()->state = COLLECT_STONE;
        Robot::instance()->state = FOLLOW_TAPE;
        return;
      case 9: 
        alignPillar();
        Robot::instance()->state = GO_HOME;
        return;
    }
  }
  else{ // methanos
    switch(Robot::instance()->splitNumber){
      case 1: case 2: case 7:
        turnRight();
        Robot::instance()->state = FOLLOW_TAPE;
        return;
      case 3: case 4: case 5: case 6: case 8: 
        alignPillar();
        //Robot::instance()->state = COLLECT_STONE;
        Robot::instance()->state = FOLLOW_TAPE;
        return;      
      case 9: 
        alignPillar();
        Robot::instance()->state = GO_HOME;
        return;
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
    leftTapeFollow = analogRead(L_TAPE_FOLLOW) >= my_THRESHOLD;
    rightTapeFollow = analogRead(R_TAPE_FOLLOW) >= my_THRESHOLD;
    leftDecide = analogRead(L_DECIDE) >= my_DECIDE_THRESHOLD;
    rightDecide = analogRead(R_DECIDE) >= my_DECIDE_THRESHOLD;

    if((leftDecide|| rightDecide) && (leftTapeFollow || rightTapeFollow) && (debounce > DEBOUNCE)) {
      if(homeSplit){
        if(Robot::instance()->TEAM){ //thanos
          turnInPlaceLeft();
          pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 600, 0);
          pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 600, 0);
          delay(1000);
        }
        else{
          turnInPlaceRight();
          pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 600, 0);
          pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 600, 0);
          delay(1000);
        }  
        dropGauntlet();
        pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 500, 0);
        pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 500, 0);
        delay(20);
        stop();
        Robot::instance()->state = PARK;
        return;
      }
    homeSplit=true;
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
    debounce++;
  }
return;
}

void TapeFollower::park(){
  pwm_stop(LEFT_FORWARD_WHEEL_MOTOR);
  pwm_stop(RIGHT_FORWARD_WHEEL_MOTOR);
  pwm_stop(LEFT_BACKWARD_WHEEL_MOTOR);
  pwm_stop(RIGHT_BACKWARD_WHEEL_MOTOR);
  pwm_stop(ARM_MOTOR_LEFT);
  pwm_stop(ARM_MOTOR_RIGHT);
  pwm_stop(ARM_MOTOR_UP);
  pwm_stop(ARM_MOTOR_DOWN);
  Robot::instance()->state = PARK;
  return;
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

void TapeFollower::alignPillar(){
  pwm_start(LEFT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0); //very slow
  pwm_start(RIGHT_FORWARD_WHEEL_MOTOR, CLOCK_FQ, MAX_SPEED, 100, 0);
  while(true){
    if(analogRead(L_ALIGN) >= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = LEFT;
      return;
    }
    else if(analogRead(R_ALIGN)>= my_ALIGN_THRESHOLD){
      stop();
      Robot::instance()->direction = RIGHT;
      return;
    }
  }
  return;
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

bool TapeFollower::multi(bool C, bool B, bool A) {
  digitalWrite(MULTIPLEX_A, A);
  digitalWrite(MULTIPLEX_B, B);
  digitalWrite(MULTIPLEX_C, C); 
  return digitalRead(MULTIPLEX_OUT);
}




