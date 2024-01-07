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

#include "include/wifiHandler.h"
#include "include/clientMQTT.h"
#include "include/robotStatusCollector.h"


void setup() {
  Serial.begin(115200);

  initializeWifiConnection();

  initializeMQTTConnection();
  establishMQTTConnection();
}


void loop() {
  checkWifiConnection();
  handleMQTTLoop();
  Serial.println("Robot Status: " + generateRobotStatusJson());
  delay(1000);
}

