#include <DMotor_mod.h>

#include "../include/movements.h"
#include "../include/physicalData.h"

// FUNCTION 1: WAYS TO MOVE THE LLUBOT GIVEN NUMBER OF MICROSTEPS: --------------------------------------------------------//
void movement (char way, float steps, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances) { // function with two entries and no return

  // First you read the option you are given:
  char way_ = way;      // 4 options: 'F'orward, 'B'ackward, 'R'ight, 'L'eft
  float steps_ = steps; // number of steps taken in the stepper motors
   orders[next_order] = way_;
   distances[next_order] = 0;
  // F = Forward
  if (way_ == 'F') {
    // Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, FORWARD, SINGLE); // both motors run forward the precised microsteps
      motorL.step(1, FORWARD, SINGLE); // to go Forward
      distances[next_order]++;
    }
  }
  // B = Backward
  if (way_ == 'B') {
    // Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, BACKWARD, SINGLE); // both motors run backward the precised microsteps
      motorL.step(1, BACKWARD, SINGLE); // to go Backward
      distances[next_order]++;
    }
  }
  // R = Right, clockwise
  if (way_ == 'R') {
    // Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, BACKWARD, SINGLE); // the right motor runs backward and the left forward
      motorL.step(1, FORWARD, SINGLE);  // to turn clockwise in the center of the wheels axis
      distances[next_order]++;
    }
  }
  // L = Left, anti-clockwise
  if (way_ == 'L') {
    // Running the LLUBot:
    for(int i = 0; i < steps_; i++){
      motorR.step(1, FORWARD, SINGLE);  // the left motor runs backward and the right forward to
      motorL.step(1, BACKWARD, SINGLE); // turn anti-clockwise in the center of the wheels axis
      distances[next_order]++;
    }
  }
  next_order++;
}
//-------------------------------------------------------------------------------------------------------------------------//

// FUNCTION 2: ADVANCE OR REVERSE GIVEN NUMBER OF CENTIMETERS: ------------------------------------------------------------//
void goStraight (char way, float cm, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances) { // function with two entries and no return
  
  // First you read the option you are given:
  char way_ = way;
  float cm_ = cm;

  // Then it initializes the involved variables:
  float steps_ = 0;

  // Choosing the cm to introduce as steps in the motor motion:
  steps_ = cm_ * stepperResolution * stepToMicrostep / (twoPi * wheelRadius); 

  // Choosing a direction for the motors:
  movement (way_, steps_, motorR, motorL, next_order, orders, distances);
}
//-------------------------------------------------------------------------------------------------------------------------//

// FUNCTION 3: TURN A CERTAIN ANGLE (IN DEGREES): -------------------------------------------------------------------------//
void turn(char way, float angle, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances) { // function with two entries and no return

  // First you read the option you are given:
  char way_ = way;
  float angle_ = angle;

  // Then it initializes the involved variables:
  float steps_ = 0.0;

  // Choosing the cm to introduce as steps in the motor motion:
  // The following is the complete relation between the steps and the angle needed:

  // steps_ = angle_ * (stepperResolution * stepToMicrostep / (twoPi * wheelRadius)) * twoPi * (wheelsAxisDis / 2) / 360;

  // And this is the reduced one for more efficiency:
  steps_ = angle_ * stepperResolution * stepToMicrostep * wheelsAxisDis / (wheelRadius * 720);

  // Choosing a direction for the motors:
  movement (way_, steps_, motorR, motorL, next_order, orders, distances);
}
