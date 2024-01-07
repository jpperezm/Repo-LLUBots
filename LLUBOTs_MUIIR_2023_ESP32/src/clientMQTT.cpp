#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "../include/clientMQTT.h"
#include "../include/robotStatusCollector.h"

WiFiClient wifiClient;

PubSubClient MQTTClient(wifiClient);

const char *mqtt_broker = "51.20.185.180";

int LLUBotID;

const char *home_topic = "/LLUBot/NFC/";
const char *config_topic = "/LLUBot/config";
const char *info_topic = "/LLUBot/info";

MQTTConnectionState mqttState = kMQTTDisconnected;
unsigned long mqttConnectStartMillis = 0;
const unsigned long mqttConnectTimeout = 10000;


void initializeMQTTConnection() {
  MQTTClient.setServer(mqtt_broker, mqtt_port);
  MQTTClient.setCallback(callback);
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
    MQTTClient.loop(); // Process MQTT client loop
  } else {
    checkMQTTConnection(); // Attempt to reconnect if disconnected
  }
}


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

  if (String(topic) == home_topic) {
    Serial.println("Going home");
    //goHome();
  }

  if (String(topic) == config_topic) {
    //processConfigCommand(message);
  }
  Serial.println();
}


void publishRobotStatus() {
  String jsonData = generateRobotStatusJson();

  if (!MQTTClient.connected()) {
    establishMQTTConnection();
  }
  MQTTClient.publish(info_topic, jsonData.c_str());
}