/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation.
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 * 
 * 
 * Modified by: MUIIR 2023 students.
 */

#include <DMotor_mod.h>
#include <Wire.h>

#include "include/movements.h"
#include "include/lineFollower.h"

//const int lowBatteryLedPin = D0;
//const int batteryVoltagePin = D4;

uint8_t speed = 100; // in rpm
uint8_t initServoAngle = 90;  // in degrees

char s;
bool isStarted = 0;

void setup() {
  Serial.begin(115200);

  initMotors(speed);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(requestEvent);
  Serial.println("Setup");
}


void loop() {
  if (isStarted) {
    sigueLineas(lecturaSensorIzq, lecturaSensorDer);
  }
}


void receiveEvent(int howMany) {
  Serial.println("Entra: ");
  s = Wire.read();
  Serial.println(s);
  switch(s) {
  case 'm': isStarted = 1; break; // marcha
  case 'p': isStarted = 0; break; // paro
  case 'g': // 180 
    break;
  case 'n': // 90
    break;
  case 'i':
    lecturaSensorIzq = Wire.read();
    Serial.print("Lectura sensor izquierdo: ");
    Serial.println(lecturaSensorIzq);
    break;
  case 'd':
    lecturaSensorDer = Wire.read();
    Serial.print("Lectura sensor derecho: ");
    Serial.println(lecturaSensorDer);
    break;
  }
}