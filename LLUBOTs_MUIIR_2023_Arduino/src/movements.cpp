#include <Arduino.h>
#include <AccelStepper.h>
#include <AFMotor.h>
#include "../include/movements.h"

AF_Stepper motorR(256, 1);    // Right motor  (stepper)
AF_Stepper motorL(256, 2);    // Left  motor  (stepper)

void forwardstep1() {  
  motorR.onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  motorR.onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motorL.onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  motorL.onestep(BACKWARD, SINGLE);
}

AccelStepper stepperR(forwardstep1, backwardstep1);
AccelStepper stepperL(forwardstep2, backwardstep2);

char orders[100];  //Orders executed
int distances[100];  //Distances moved in each order
int next_order = 0;  //the next order and distance to fill

void initMotors(float speed) {
    stepperR.setMaxSpeed(speed);
    stepperR.setAcceleration(200.0);
    stepperR.moveTo(1000000);
    
    stepperL.setMaxSpeed(speed);
    stepperL.setAcceleration(200.0);
    stepperL.moveTo(1000000);
}

void movement(char way) {
  //orders[next_order] = way;
  //distances[next_order] = 0;
  /*
  if (way == 'F' && movementRequest == 1) {
    motorR.step(1, FORWARD, SINGLE);
    motorL.step(1, FORWARD, SINGLE);
    //distances[next_order]++;
  } else if (way == 'B' && backMovementRequest == 1) {
    motorR.step(1, BACKWARD, SINGLE);
    motorL.step(1, BACKWARD, SINGLE);
    //distances[next_order]++;
  } else if (way == 'R' && rightMovementRequest == 1) {
    //motorR.step(1, BACKWARD, SINGLE);
    motorL.step(1, FORWARD, SINGLE);
    //distances[next_order]++;
  } else if (way == 'L' && leftMovementRequest == 1) {
    motorR.step(1, FORWARD, SINGLE);
    //motorL.step(1, BACKWARD, SINGLE);
    //distances[next_order]++;
  }*/

  if (movementRequest == 1){
    if (way == 'F') {
      Serial.println("Going forward");
      stepperR.run();
      stepperL.run();
      //distances[next_order]++;
    } else if (way == 'B') {
      Serial.println("Going back");
      stepperR.run();
      stepperL.run();
      //distances[next_order]++;
    } else if (way == 'R') {
      //motorR.step(1, BACKWARD, SINGLE);
      Serial.println("Going right");
      stepperL.run();
      //distances[next_order]++;
    } else if (way == 'L') {
      Serial.println("Going left");
      stepperR.run();
      //motorL.step(1, BACKWARD, SINGLE);
      //distances[next_order]++;
    }
  }

  //next_order++;

      // Determinar el movimiento según 'way'
  /*
  if (way == 'F') {
      while (movementRequest) {
          Serial.println("Going forward");
          motorR.step(1, FORWARD, SINGLE);
          motorL.step(1, FORWARD, SINGLE);
          // Pausa opcional entre pasos si es necesaria
          // delay(100); // Por ejemplo, si se requiere una pausa entre pasos
      }
  } else if (way == 'B') {
      while (movementRequest) {
          Serial.println("Going back");
          motorR.step(1, BACKWARD, SINGLE);
          motorL.step(1, BACKWARD, SINGLE);
          // Pausa opcional entre pasos si es necesaria
          // delay(100); // Por ejemplo, si se requiere una pausa entre pasos
      }
  } else if (way == 'R') {
      while (rightMovementRequest) {
          //motorR.step(1, BACKWARD, SINGLE);7
          Serial.println("Going right");
          motorL.step(1, FORWARD, SINGLE);
          // Pausa opcional entre pasos si es necesaria
          // delay(100); // Por ejemplo, si se requiere una pausa entre pasos
      }
  } else if (way == 'L') {
      while (leftMovementRequest) {
          Serial.println("Going left");
          motorR.step(1, FORWARD, SINGLE);
          //motorL.step(1, BACKWARD, SINGLE);
          // Pausa opcional entre pasos si es necesaria
          // delay(100); // Por ejemplo, si se requiere una pausa entre pasos
      }
  }*/
}

void goStraight (char way, float cm) { 
  float steps = 0;
  // Choosing the cm to introduce as steps in the motor motion:
  steps = cm * stepperResolution * stepToMicrostep / (twoPi * wheelRadius); 
  movement (way);
}


void turn(char way, float angle) {
  float steps = 0.0;
  // Choosing the cm to introduce as steps in the motor motion:
  steps = angle * stepperResolution * stepToMicrostep * wheelsAxisDis / (wheelRadius * 720);  // Magic numbers are not nice
  movement (way);
}
