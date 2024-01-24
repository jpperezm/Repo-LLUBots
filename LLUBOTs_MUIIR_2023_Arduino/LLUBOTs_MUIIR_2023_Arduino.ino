/** Universidad de La Laguna
  * Escuela de Doctorado y Estudios de Postgrado
  * Máster Universitario en Informática Industrial y Robótica
  * Robótica Inteligente y Sistemas Autónomos
  *
  * @file LLUBOTS_MUIIR_2023_Arduino.ino
  * 
  * @date 19-01-24
  * @brief Autonomous Robot Communication and Navigation System
  *
  * This Arduino sketch is designed to work as a slave control system for the LLUBOTS project. 
  * Its primary function is to execute commands received from the ESP32 master controller. 
  * It handles real-time sensor data processing and motor control, enabling the LLUBOTS to 
  * navigate autonomously through assigned paths towards their 'homes'.
  * 
  * The sketch is responsible for interpreting sensor signals, managing collision avoidance, 
  * and executing precise movements as directed by the ESP32. This allows for a distributed 
  * approach to control, where the ESP32 handles high-level decision-making and communication, 
  * while the Arduino ensures responsive and accurate physical interactions with the environment.
  * 
  * Key features include sensor data acquisition, real-time motor control, and integration 
  * with the ESP32 for coordinated multi-robot tasks. The ultimate aim is to achieve a 
  * harmonious balance between autonomous navigation and collaborative task fulfillment among 
  * the LLUBOTS.
  */


#include <Wire.h>
#include <AccelStepper.h>
#include <AFMotor.h>


// Initialize stepper motors for right and left wheels
AF_Stepper rightMotorStepper(256, 1);
AF_Stepper leftMotorStepper(256, 2);


// Function definitions for moving the right motor forward and backward
void stepRightMotorForward() {  
  rightMotorStepper.onestep(FORWARD, SINGLE);
}
void stepRightMotorBackward() {  
  rightMotorStepper.onestep(BACKWARD, SINGLE);
}


// Function definitions for moving the left motor forward and backward
void stepLeftMotorForward() {  
  leftMotorStepper.onestep(FORWARD, SINGLE);
}
void stepLeftMotorBackward() {  
  leftMotorStepper.onestep(BACKWARD, SINGLE);
}


// Initialize AccelStepper objects for both motors
AccelStepper stepperR(stepRightMotorForward, stepRightMotorBackward);
AccelStepper stepperL(stepLeftMotorForward, stepLeftMotorBackward);


// Set initial variables for speed, line following, and turning
float motorSpeedRPM = 300.0;
bool isLineFollowerActivated = false;
bool isInitiateRightTurn = false;
volatile char receivedCommand = ' ';
int turnTime = 1000;  // in milliseconds


// Variables to store IR sensor readings
int leftIRSensorRead = 0;
int rightIRSensorRead = 0;


// Initialize motors with specified speed and acceleration
void initializeMotors(float speed) {
    stepperR.setMaxSpeed(speed);
    stepperR.setAcceleration(200.0);
    stepperR.moveTo(1000000);
    
    stepperL.setMaxSpeed(speed);
    stepperL.setAcceleration(200.0);
    stepperL.moveTo(1000000);
}


void setup() {
  Serial.begin(9600);

  initializeMotors(motorSpeedRPM);

  Wire.begin(8);  // Start I2C communication on address 8
  Wire.onReceive(receiveEvent);  // Register function to handle received I2C data
}


void loop() {
  unsigned long now = millis();
  static unsigned long lastTurnTime = 0;

  // Logic for line following and turning
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
  if (isInitiateRightTurn) {
    if (now - lastTurnTime > turnTime) {
      lastTurnTime = now;
      if (leftIRSensorRead == 0 && rightIRSensorRead == 1) {
        isInitiateRightTurn = false;
        isLineFollowerActivated = true;
      }
    }
    stepperL.run();
  }
}


// Function to handle received I2C data
void receiveEvent() {
  receivedCommand = Wire.read();

  if (receivedCommand == 'g') {
    isLineFollowerActivated = true;
  }
  if (receivedCommand == 's') {
    isLineFollowerActivated = false;
  }
  if (receivedCommand == 'r') {
    rightIRSensorRead = Wire.read();
  }
  if (receivedCommand == 'l') {
    leftIRSensorRead = Wire.read();
  }
  if (receivedCommand == 't') {
    isLineFollowerActivated = false;
    isInitiateRightTurn = true;
  }
}