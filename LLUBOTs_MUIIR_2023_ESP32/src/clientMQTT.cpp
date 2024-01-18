#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "../include/clientMQTT.h"
#include "../include/robotStatusCollector.h"
#include "../include/robotStateMachine.h"

WiFiClient wifiClient;

PubSubClient MQTTClient(wifiClient);

const char *mqtt_broker = "51.20.185.180";
const int mqtt_port = 1883;

int LLUBotID;

const char *home_topic = "/LLUBot/NFC/";
const char *config_topic = "/LLUBot/config/";
const char *info_topic = "/LLUBot/info";
const char *roundabout_topic = "/LLUBot/roundabout";

MQTTConnectionState mqttState = kMQTTDisconnected;
unsigned long mqttConnectStartMillis = 0;
const unsigned long mqttConnectTimeout = 10000;


String convertPayloadToString(byte *payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  return message;
}


void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  String message = convertPayloadToString(payload, length);

  if (String(topic) == "START") {
    startCommandReceived = true;
  } else if (String(topic) == "RESET") {
    resetCommandReceived = true;
  } else if (String(topic) == home_topic + LLUBotID) {
    goalStreetName = message.toInt();
  } else if (String(topic) == roundabout_topic) {
    numberOfLLUBotsOnRoundabout++;  // Assuming my own message arrives
  } else if (String(topic) == String(config_topic) + "numLLUBots") {
    numberOfLLUBots = message.toInt();
  } else if (String(topic) == String(config_topic) + String(LLUBotID) + "/home") {
    homeName = message.toInt();
  } else if (String(topic) == String(config_topic) + String(LLUBotID) + "/initialStreet") {
    initialStreetName = message.toInt();
  }

  Serial.println();
}


void initializeMQTTConnection() {
  MQTTClient.setServer(mqtt_broker, mqtt_port);
  MQTTClient.setCallback(callback);
  establishMQTTConnection();
}


void establishMQTTConnection() {
  if (mqttState == kMQTTDisconnected) {
    Serial.println("Connecting to MQTT...");
    mqttConnectStartMillis = millis();
    mqttState = kMQTTConnecting;
  } else if (mqttState == kMQTTConnecting) {
    if (MQTTClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
      MQTTClient.subscribe(home_topic + LLUBotID);
      MQTTClient.subscribe(config_topic);
      MQTTClient.subscribe(config_topic + LLUBotID);
      MQTTClient.subscribe(roundabout_topic);
      MQTTClient.subscribe("START");
      MQTTClient.subscribe("RESET");
      mqttState = kMQTTConnected;
    } else {
      Serial.print("Failed to connect to MQTT, state: ");
      Serial.println(MQTTClient.state());
      if (millis() - mqttConnectStartMillis > mqttConnectTimeout) {
        Serial.println("Failed to connect to MQTT within the timeout period.");
        mqttState = kMQTTDisconnected;
      }
    }
  }
}


void checkMQTTConnection() {
  if (mqttState != kMQTTConnected) {
    establishMQTTConnection();
  }
}


void handleMQTTLoop() {
  if (MQTTClient.connected()) {
    MQTTClient.loop();
  } else {
    mqttState = kMQTTDisconnected;
    checkMQTTConnection();
  }
}


void publishRobotStatus() {
  String jsonData = generateRobotStatusJson();

  if (!MQTTClient.connected()) {
    establishMQTTConnection();
  }
  MQTTClient.publish(info_topic, jsonData.c_str());
}


void publishRoundabout(String status) {
  if (!MQTTClient.connected()) {
    establishMQTTConnection();
  }
  MQTTClient.publish(roundabout_topic, status.c_str());
}


void publishLLUBotHomeLocation(int homeID) {
  if (!MQTTClient.connected()) {
    establishMQTTConnection();
  }
  MQTTClient.publish(home_topic + homeID, String(initialStreetName).c_str());
  LLUBotLocationSent = true;
}