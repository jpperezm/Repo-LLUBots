/** Universidad de La Laguna
  * Escuela de Doctorado y Estudios de Postgrado
  * Máster Universitario en Informática Industrial y Robótica
  * Robótica Inteligente y Sistemas Autónomos
  *
  * @file LLUBOTS_MUIIR_2023_ESP32.ino
  * 
  * @date 19-01-24
  * @brief Autonomous Robot Communication and Navigation System
  *
  * This program is the central part of a system designed for autonomous robot communication 
  * and navigation. It focuses on enabling LLUBOTS to interact with each other to complete a 
  * shared task. Each robot is randomly assigned a path and tasked with finding a 'home' location. 
  * Upon finding a home, it determines the corresponding robot and sends a navigational message. 
  * If a robot finds its own home, it remains there. After messaging, robots return to their 
  * starting position and wait for others to complete their search.
  * 
  * This system uses an ESP32 as the master controller, handling all high-level decision-making 
  * and communication. The Arduino acts as a slave, receiving sensor data from the master and 
  * controlling motor movements accordingly. The goal is to achieve seamless coordination among 
  * the robots while avoiding collisions and efficiently completing the task.
  */


#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>

#include "include/wifiHandler.h"
#include "include/clientMQTT.h"
#include "include/LLUBotStatusCollector.h"
#include "include/LLUBotStateMachine.h"
#include "include/sensorsHandler.h"
#include "include/I2CHandler.h"


void setup() {
  Serial.begin(115200);

  initializeWifiConnection();
  initializeMQTTConnection();

  initializeI2C();
  initializeSensors();
}


void loop() {
  unsigned long now = millis();
  static unsigned long lastMQTTMessage = 0;
  static unsigned long lastI2CCommunication = 0;
  
  // Regularly check WiFi and MQTT connection and handle the LLUBot's state.
  checkWifiConnection();
  handleMQTTLoop();
  handleRobotState();
  updateRobotState();
  
  // Publish the LLUBot's status every 10 seconds.
  if (now - lastMQTTMessage > 10000) {
    lastMQTTMessage = now;
    publishRobotStatus();
  }

  // Update sensor readings.
  updateSensors();

  // Send sensor data via I2C every 200 milliseconds.
  if (now - lastI2CCommunication > 200) {
    lastI2CCommunication = now;
    sendLeftSensorData(getLeftIRValue());
    sendRightSensorData(getRightIRValue());
  }

  if (getUltrasonicDistance() < 10) {
    emergencyStop = true;
  } else {
    emergencyStop = false;
  }
}
