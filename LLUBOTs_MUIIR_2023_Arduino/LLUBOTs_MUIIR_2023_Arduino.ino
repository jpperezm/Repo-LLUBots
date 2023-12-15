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
#include "include/mainClient.h"
#include "include/roombaClient.h"
#include "include/distanceSensing.h"
#include "include/movements.h"
#include "include/roomba.h"
#include "include/serverConfig.h"
#include "include/clientMQTT.h"

const int lowBatteryLedPin = D0;
const int batteryVoltagePin = D4;
const int servoPin = D7;

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
  /*lowBatteryMock = 1; // TODO Low battery control using batteryVoltagePin

  if (lowBatteryMock == 0) {
    digitalWrite(lowBatteryLedPin, HIGH);
    lowBatteryClient();
  } else {
    switch (selectedActivity) {
      case 1:
        Serial.println("hola");
        roombaGame();
        break;
      default:
        mainMenu();
        break;
    }
  }*/
}
