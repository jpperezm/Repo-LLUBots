#include <Arduino.h>
#include <String.h>
#include <ESP8266WiFi.h>

#include "../include/lowBatteryClient.h"
#include "../include/serverConfig.h"

int lowBatteryMock = 1; 

void lowBatteryClient() {
  WiFiClient client;
  waitForClient(client);
  String request = readClientRequest(client);

  client.println(lowBatteryPage);

  Serial.println("Client disconnected");
  Serial.println();
}