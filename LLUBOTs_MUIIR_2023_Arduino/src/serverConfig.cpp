#include <ESP8266WiFi.h>

#include "../include/serverConfig.h"

const char* ssid = "Redmi Note 12 Pro 5G";
const char* password = "sonawifi"; 

WiFiServer server(80);


void connectWifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Use this URL: http://");
  Serial.println(WiFi.localIP().toString() + "/");
}


void startServer() {
  server.begin();
  Serial.println("Server started");
}


void waitForClient(WiFiClient &client) {
  client = server.available();
  while (!client) {
    client = server.available();
  }

  Serial.println("new client");
  while (!client.available()) {
      delay(1);
  }
}


String readClientRequest(WiFiClient &client) {
  String request = client.readStringUntil('\r');
  Serial.println(request);
  return request;
}
