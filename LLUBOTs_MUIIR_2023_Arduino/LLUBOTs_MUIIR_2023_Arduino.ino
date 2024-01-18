/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation.
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 * 
 * 
 * Modified by: MUIIR 2023 students.
 */
 
#include <Wire.h>
#include <AccelStepper.h>
#include <AFMotor.h>

#include "include/movements.h"
#include "include/lineFollower.h"

float speed = 400.0; // in rpm
uint8_t initServoAngle = 90;  // in degrees
bool isLineFollowerActivated = false;

void setup() {
  Serial.begin(9600);

  initMotors(speed);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(requestEvent);
}


void loop() {
  lineFollower(leftIRSensorRead, rightIRSensorRead);
}


void receiveEvent() {
  char characterCommand = Wire.read();
  int value = 0; 
  switch (characterCommand) {
    case 'm':
      Serial.println("Marcha");
      isLineFollowerActivated = true;
      break;   
    case 'p':
      Serial.println("Paro");
      isLineFollowerActivated = false;
      break;
    case 'r': 
      rightIRSensorRead = Wire.read();
      Serial.print("r: ");
      Serial.println(String(value));
      break;
    case 'l':
      leftIRSensorRead = Wire.read();
      Serial.print("l: ");
      Serial.println(String(value));
      break;      
    case 'g': //180
      Serial.println("Giro 180");
      break;
    case 'n': //90
      Serial.println("Giro 90");
      break;
  }
}