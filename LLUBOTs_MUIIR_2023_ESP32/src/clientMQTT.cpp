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
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  String message = convertPayloadToString(payload, length);

  if (String(topic) == "START") {
    startCommandReceived = true;
  } else if (String(topic) == "RESET") {
    resetCommandReceived = true;
  } else if (String(topic) == String(home_topic) + String(homeName)) {
    Serial.println("Llega goal");
    goalStreetName = message.toInt();
  } else if (String(topic) == roundabout_topic) {
    Serial.println("Alguien llegó a la rotonda");
    numberOfLLUBotsOnRoundabout++;
    Serial.print("Número de llubots en la rotonda: ");
    Serial.println(numberOfLLUBotsOnRoundabout);
  } else if (String(topic) == String(config_topic) + "numLLUBots") {
    Serial.print("Numero de llubots");
    Serial.println(String(message.toInt()));
    numberOfLLUBots = message.toInt();
  } else if (String(topic) == String(config_topic) + String(LLUBotID) + "/home") {
    homeName = message.toInt();
    MQTTClient.disconnect();
  } else if (String(topic) == String(config_topic) + String(LLUBotID) + "/initialStreet") {
    initialStreetName = message.toInt();
  } else if (String(topic) == "PRUEBA_SIGUELINEAS") {
    if (message == "START") {
      lineFollowerTest = true;
    } else if (message == "STOP") {
      lineFollowerTest = false;
    }
  } else if (String(topic) == config_topic) {
    configModeActivated = true;
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
    if (MQTTClient.connect(LLUBotID.c_str())) {
      Serial.println("Connected to MQTT");
      if (homeName != -1) {
        MQTTClient.subscribe(String(String(home_topic) + String(homeName)).c_str());
      }
      MQTTClient.subscribe(config_topic);
      MQTTClient.subscribe(String(String(config_topic) + LLUBotID + "/home").c_str());
      MQTTClient.subscribe(String(String(config_topic) + LLUBotID.c_str() + "/initialStreet").c_str());
      MQTTClient.subscribe(String(String(config_topic) + "numLLUBots").c_str());
      MQTTClient.subscribe(roundabout_topic);
      MQTTClient.subscribe("START");
      MQTTClient.subscribe("RESET");
      MQTTClient.subscribe("PRUEBA_SIGUELINEAS");
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


void publishLLUBotHomeLocation(int homeName) {
  if (!MQTTClient.connected()) {
    establishMQTTConnection();
  }
  MQTTClient.publish(String(String(home_topic) + String(homeName)).c_str(), String(initialStreetName).c_str());
  LLUBotLocationSent = true;
}


void MQTTdisconnect() {
  MQTTClient.disconnect();
}