#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../include/clientMQTT.h"
#include "../include/movements.h"

WiFiClient wifiClient;

PubSubClient MQTTClient(wifiClient);

const char *mqtt_broker = "192.168.84.74";
const char *movement_topic = "LLUBot/movement";

// Initialize the Wi-Fi client
void mqttSetup() {
  MQTTClient.setServer(mqtt_broker, mqtt_port);
  MQTTClient.setCallback(callback);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
  // Convert payload to String
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  // Check if the message is for the movement topic
  if (String(topic) == movement_topic) {
    if (message == "forward") {
      Serial.println("Moving forward");
      goStraight('F', 10);
    } else if (message == "backward") {
      Serial.println("Moving backward");
      goStraight('B', 10);
    } else if (message == "left") {
      Serial.println("Turning left");
      turn('L', 90);
    } else if (message == "right") {
      Serial.println("Turning right");
      turn('R', 90);
    } else if (message == "stop") {
      Serial.println("Stopping");
      stop();
    }
  }
  Serial.println();
}

void connect() {
  while (!MQTTClient.connected()) {
    Serial.println("Connecting to MQTT...");

    if (MQTTClient.connect("ESP8266Client")) {
      Serial.println("connected");
      MQTTClient.subscribe(movement_topic);
    } else {
      Serial.print("failed with state ");
      Serial.print(MQTTClient.state());
      delay(2000);
    }
  }
}