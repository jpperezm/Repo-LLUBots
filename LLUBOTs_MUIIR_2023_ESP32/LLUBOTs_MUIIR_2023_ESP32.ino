/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation.
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 * 
 * 
 * Modified by: MUIIR 2023 students.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>

#include "include/wifiHandler.h"
#include "include/clientMQTT.h"
#include "include/robotStatusCollector.h"
#include "include/robotStateMachine.h"
#include "include/sensorsHandler.h"
#include "include/I2CSensorHandler.h"


void setup() {
  Serial.begin(115200);

  initializeWifiConnection();
  initializeMQTTConnection();

  initializeI2CSensors();
  initializeSensors();
  sendLineFollowerCommand();
}


void loop() {
  unsigned long now = millis();
  static unsigned long lastMsg = 0;
  static unsigned long lastI2C = 0;
  if (!lineFollowerTest) {
    checkWifiConnection();
    handleMQTTLoop();
    handleRobotState();
    updateRobotState();
    if (now - lastMsg > 10000) {
      lastMsg = now;
      publishRobotStatus();
    }
  }

  updateSensors();

  if (now - lastI2C > 200) {
    lastI2C = now;
    sendLeftSensorData(getLeftIRValue());
    sendRightSensorData(getRightIRValue());
  }

  if (getUltrasonicDistance() < 10) {
    sendStopCommand();
  }
}
