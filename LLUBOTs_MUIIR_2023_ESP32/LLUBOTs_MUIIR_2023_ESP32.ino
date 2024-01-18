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
}


void loop() {
  unsigned long now = millis();
  static unsigned long lastMsg = 0;
  checkWifiConnection();
  handleMQTTLoop();
  handleRobotState();
  updateRobotState();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    publishRobotStatus();
  }
}
