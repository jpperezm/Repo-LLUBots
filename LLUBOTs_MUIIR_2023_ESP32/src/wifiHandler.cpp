#include <WiFi.h>

#include "../include/wifiHandler.h"


// WiFi credentials

const char* wifiSSID = "ULL-LLUBot-WiFi";
const char* wifiPassword = "password";


// Variables to manage WiFi connection attempts

unsigned long wifiConnectStartMillis = 0;
const unsigned long wifiConnectTimeout = 10000;


// Attempts to establish a WiFi connection using the specified credentials.
void attemptWifiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    WiFi.begin(wifiSSID, wifiPassword);
    wifiConnectStartMillis = millis();
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP().toString());
  } else {
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP().toString());
  }
}


// Initializes the WiFi connection.
void initializeWifiConnection() {
  attemptWifiConnection();
}


// Checks the status of the WiFi connection and attempts reconnection if necessary.
void checkWifiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifiConnectStartMillis > wifiConnectTimeout) {
      WiFi.disconnect();
      attemptWifiConnection();
    }
  }
}
