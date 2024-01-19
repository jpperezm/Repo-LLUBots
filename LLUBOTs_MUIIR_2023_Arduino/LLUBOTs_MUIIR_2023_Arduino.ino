#include <Wire.h>
#include <AccelStepper.h>
#include <AFMotor.h>

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

float speed = 300.0; // in rpm
bool isLineFollowerActivated = false;
bool turnRight = false;
volatile char characterCommand = ' ';
int TurnTime = 1000;

int leftIRSensorRead = 0;
int rightIRSensorRead = 0;

void initMotors(float speed) {
    stepperR.setMaxSpeed(speed);
    stepperR.setAcceleration(200.0);
    stepperR.moveTo(1000000);
    
    stepperL.setMaxSpeed(speed);
    stepperL.setAcceleration(200.0);
    stepperL.moveTo(1000000);
}

void setup() {
  Serial.begin(9600);

  initMotors(speed);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}


void loop() {
  unsigned long now = millis();
  static unsigned long lastTurnTime = 0;

  if (isLineFollowerActivated) {
    if (leftIRSensorRead == 0 && rightIRSensorRead == 1) {
      stepperL.run();
    }
    if (leftIRSensorRead == 1 && rightIRSensorRead == 0) {
      stepperR.run();
    }
    if (leftIRSensorRead == 1 && rightIRSensorRead == 1) {
      stepperR.run();
      stepperL.run();
    }
    if (leftIRSensorRead == 0 && rightIRSensorRead == 0) {
      stepperR.run();
    }
  }
  if (turnRight) {
    if (now - lastTurnTime > TurnTime) {
      lastTurnTime = now;
      if (leftIRSensorRead == 0 && rightIRSensorRead == 1) {
        turnRight = false;
        isLineFollowerActivated = true;
      }
    }
    stepperL.run();
  }
}


void receiveEvent() {
  characterCommand = Wire.read();

  if (characterCommand == 'g') {
    isLineFollowerActivated = true;
  }
  if (characterCommand == 's') {
    isLineFollowerActivated = false;
  }
  if (characterCommand == 'r') {
    rightIRSensorRead = Wire.read();
  }
  if (characterCommand == 'l') {
    leftIRSensorRead = Wire.read();
  }
  if (characterCommand == 't') {
    isLineFollowerActivated = false;
    turnRight = true;
  }
}