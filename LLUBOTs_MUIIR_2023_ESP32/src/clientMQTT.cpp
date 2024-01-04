#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../include/clientMQTT.h"
#include "../include/movements.h"

WiFiClient wifiClient;

PubSubClient MQTTClient(wifiClient);

const char *mqtt_broker = "51.20.185.180";

int LLUBotID;

const char *movement_topic = "/LLUBot/movement/";
const char *home_topic = "/LLUBot/NFC/"
const char *config_topic = "/LLUBot/config";
const char *mode_topic = "/LLUBot/mode/";

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
  String message = convertPayloadToString(payload, length);
  if (String(topic) == movement_topic) {
    processMovementCommand(message);
  }

  if (String(topic) == home_topic) {
    Serial.println("Going home");
    //goHome();
  }

  if (String(topic) == mode_topic) {
    Serial.println("Changing mode");
    //changeMode(message);
  }

  if (String(topic) == config_topic) {
    config(message);
  }
  Serial.println();
}

String convertPayloadToString(byte *payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  return message;
}

void config(String message) {
  if (message == "all") {
    Serial.println("config all");
    waitForID();
  } else if (LLUBotID != NULL) {
    if (message == LLUBotID) {
      Serial.println("config " + String(LLUBotID));
      waitForID();
    }
  }
}

void processMovementCommand(const String &message) {
  if (message == "forward") {
    Serial.println("Moving forward");
    //goStraight('F', 10);
  } else if (message == "backward") {
    Serial.println("Moving backward");
    //goStraight('B', 10);
  } else if (message == "left") {
    Serial.println("Turning left");
    //turn('L', 90);
  } else if (message == "right") {
    Serial.println("Turning right");
    //turn('R', 90);
  } else if (message == "stop") {
    Serial.println("Stopping");
    //stop();
  }
}

void waitForID() {
  int mockNFC = 1;
  while (LLUBotID == NULL) {
    Serial.println("Waiting for ID");
    LLUBotID = mockNFC;
    delay(1000);
  }
}

void connect() {
  while (!MQTTClient.connected()) {
    Serial.println("Connecting to MQTT...");

    if (MQTTClient.connect("ESP8266Client")) {
      Serial.println("connected");
      MQTTClient.subscribe(movement_topic + LLUBotID);
      MQTTClient.subscribe(home_topic + LLUBotID);
      MQTTClient.subscribe(mode_topic + LLUBotID);
      MQTTClient.subscribe(config_topic);
    } else {
      Serial.print("failed with state ");
      Serial.print(MQTTClient.state());
      delay(2000);
    }
  }
}