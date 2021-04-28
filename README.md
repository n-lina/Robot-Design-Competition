<pre> Meet the team! 
 From left to right: Michelle, Thien, Karina, and me, Lina. </pre> 
<img src="https://github.com/n-lina/Robot-Design-Competition/blob/master/team.jpeg?raw=true" width="600"/>

# Overview

In the summer of 2019, my team and I competed in an Avengers themed robot design competition, for which we created a fully functional robot capable of autonomous navigation and 'Infinity stone' collection!

**The Challenge**: Two robots, one representing Thanos and the other representing Methanos, chosen at random, compete to collect the Infinity stones and the Avengers plushies. Stealing from the other team's gauntlet is fair play!

2 points are scored for every Infinity stone successfully returned to the gauntlet. 1 point is scored for every Avenger plushie 'rescued.' Collisions with the other robot, restarts, and manual assistance all result in point deductions. 

<pre> Competition Surface </pre> 

<img src="https://github.com/n-lina/Robot-Design-Competition/blob/master/surface.png?raw=true" width="600"/>

# Software Breakdown 
The software engineer of the team, I implemented accurate PID control algorithms and a round-robin state-machine software architecture that prioritizes efficiency, robustness, and code safety.

### Overall Round-Robin State-Machine Architecture 
-  The infinite loop features a state machine with 7 states: 

```cpp
void loop() {
  switch (Robot::instance()->state){
    case GO_DISTANCE: // Go a certain distance without checking for tabs/splits
      robot.goDistance(START_DETECTION);
      break;
    case FOLLOW_TAPE: // Follow Tape checking splits/tabs
      robot.followTape();
      break;
    case AVOID_COLLISION: // Avoid Collision {4}
      robot.avoidCollision();
      break; 
    case SPLIT_CHOOSER: // Split Chooser 
      robot.splitDecide();
      break;
    case COLLECT_STONE: // Collect Stone {3}
      Robot.collectStone();
      break;
    case GO_HOME: // Go home and deposit stones at certain time (1:30) {2} turn off collision interrupts 
      robot.goHome();
      break;
    case PARK: // park at gauntlet
      robot.park();
      break;
  }
}
```
The state-machine breaks the complicated, multi-variable design problem down into manageable, discrete states in which the robot's behaviour is clearly defined. Given more time, possible improvements include using interrupts or even a real-time operating system for increased control, safety, and robustness. 

### TapeFollower.cpp — PID Control Algorithm, Autonomous Navigation
-  The more accurate our tape following, the less we had to worry about falling off the edge or getting lost on the track. As such, this component of the software was imperative to our success. Dedicating 3 phototransistors to tape following, we achieved highly accurate tape following via a **Proportional-Integral-Derivative (PID) control algorithm!**
-  **Notable methods:** followTape(), turnLeft(), turnRight(), splitDecide(), avoidCollision(), alignPillar(), getPosition(), dropGauntlet()
-  This library is primarily responsible for the robot's autonomous navigation, position tracking, decision making, and alignment. 

### ManageStone.cpp
-  **Notable methods:** collectStone(), dropInStorage()
-   This library includes methods that implement the robot's ability to collect Infinity stones and drop them in the gauntlet. 

### Robot.cpp 
-   Setting up the robot; intializing variables 

# Design Process 
Long, long days (and nights) were spent in the lab ... 
<pre> A typical day in the lab ... at 10pm ..  </pre> 
<img src="https://github.com/n-lina/Robot-Design-Competition/blob/master/lab.jpeg?raw=true" width="500"/>

<pre> One Possible STM32 Bluepill Microcontroller IO Pin Layout </pre>
<img src="https://github.com/n-lina/Robot-Design-Competition/blob/master/image0.jpeg?raw=true" width="500"/>

# Results 
Making **critical optimization decisions** considering the strict time constraint, we decided to focus most on collecting stones, avoiding collisions, and accurate tape following. As a result, we were among the **top three teams in time trials.**

<br>

<pre> Our robot in action </pre>

https://youtu.be/Xdj1wyurvZQ

<br>

<pre> Competition Day </pre> 
<img src="https://github.com/n-lina/Robot-Design-Competition/blob/master/competition.jpeg?raw=true" width="500"/>

