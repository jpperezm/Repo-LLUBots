/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation.
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 * 
 * 
 * Modified by: MUIIR 2023 students.
 */

#include <PubSubClient.h>
#include <DMotor_mod.h>

#include "include/movements.h"
#include "include/lineFollower.h"

const int lowBatteryLedPin = D0;
const int batteryVoltagePin = D4;

uint8_t speed = 100; // in rpm
uint8_t initServoAngle = 90;  // in degrees

void setup() {
  Serial.begin(115200);
 
  pinMode(batteryVoltagePin, INPUT);
  pinMode(lowBatteryLedPin, OUTPUT);

  initMotors(speed);

  digitalWrite(lowBatteryLedPin, LOW);
}


void loop() {
  Serial.print("Izq: ");
  Serial.println(lecturaSensorIzq);
  Serial.print("Der: ");
  Serial.println(lecturaSensorDer);

  lecturaSensorIR();
  sigueLineas(lecturaSensorIzq, lecturaSensorDer);
}
