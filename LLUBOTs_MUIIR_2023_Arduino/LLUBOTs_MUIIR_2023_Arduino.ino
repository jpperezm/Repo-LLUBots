/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation.
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 * 
 * 
 * Modified by: MUIIR 2023 students.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DMotor_mod.h>
#include <Servo_ESP8266.h>

#include "include/lowBatteryClient.h"
#include "include/distanceSensing.h"
#include "include/movements.h"
#include "include/serverConfig.h"
#include "include/clientMQTT.h"
#include "include/lineFollower.h"

const int lowBatteryLedPin = D0;
const int batteryVoltagePin = D4;


uint8_t speed = 100; // in rpm
uint8_t initServoAngle = 90;  // in degrees

void setup() {
  Serial.begin(115200);
 
  pinMode(batteryVoltagePin, INPUT);
  pinMode(lowBatteryLedPin, OUTPUT);

  initSensorServo(servoPin, initServoAngle);
  
  connectWifi();

  //startServer();

  initMotors(speed);

  digitalWrite(lowBatteryLedPin, LOW);

  mqttSetup();
  connect();
}


void loop() {
  MQTTClient.loop();

  Serial.print("Izq: ");
  Serial.println(lecturaSensorIzq);
  Serial.print("Der: ");
  Serial.println(lecturaSensorDer);

  lecturaSensorIR();
  sigueLineas(lecturaSensorIzq, lecturaSensorDer);

}
