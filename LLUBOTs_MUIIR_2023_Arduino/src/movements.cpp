#include <DMotor_mod.h>

#include "../include/movements.h"

AF_Stepper motorR(256, 1);    // Right motor  (stepper)
AF_Stepper motorL(256, 2);    // Left  motor  (stepper)

char orders[100];  //Orders executed
int distances[100];  //Distances moved in each order
int next_order = 0;  //the next order and distance to fill


void initMotors(uint16_t speed) {
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
}


void movement(char way, float steps) {
  orders[next_order] = way;
  distances[next_order] = 0;
  if (way == 'F') {
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE);
      motorL.step(1, FORWARD, SINGLE);
      distances[next_order]++;
    }
  } else if (way == 'B') {
    for(int i = 0; i < steps; i++){
      motorR.step(1, BACKWARD, SINGLE);
      motorL.step(1, BACKWARD, SINGLE);
      distances[next_order]++;
    }
  } else if (way == 'R') {
    for(int i = 0; i < steps; i++){
      motorR.step(1, BACKWARD, SINGLE);
      motorL.step(1, FORWARD, SINGLE);
      distances[next_order]++;
    }
  } else if (way == 'L') {
    for(int i = 0; i < steps; i++){
      motorR.step(1, FORWARD, SINGLE);
      motorL.step(1, BACKWARD, SINGLE);
      distances[next_order]++;
    }
  }
  next_order++;
}


void goStraight (char way, float cm) { 
  float steps = 0;
  // Choosing the cm to introduce as steps in the motor motion:
  steps = cm * stepperResolution * stepToMicrostep / (twoPi * wheelRadius); 
  movement (way, steps);
}


void turn(char way, float angle) {
  float steps = 0.0;
  // Choosing the cm to introduce as steps in the motor motion:
  steps = angle * stepperResolution * stepToMicrostep * wheelsAxisDis / (wheelRadius * 720);  // Magic numbers are not nice
  movement (way, steps);
}
