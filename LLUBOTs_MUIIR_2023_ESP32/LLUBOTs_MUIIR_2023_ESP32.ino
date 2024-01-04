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

#include "include/serverConfig.h"
#include "include/clientMQTT.h"



void setup() {
  Serial.begin(115200);
  
  connectWifi();

  initMotors(speed);

  mqttSetup();
  connect();
}


void loop() {
  MQTTClient.loop();
}

