#include <WiFi.h>

#include "../include/wifiHandler.h"

const char* wifiSSID = "MiFibra-4891";
const char* wifiPassword = "wificasanorobes";

WifiConnectionState wifiState = kDisconnected;
unsigned long wifiConnectStartMillis = 0;
const unsigned long wifiConnectTimeout = 10000;


void initializeWifiConnection() {
  if (wifiState == kDisconnected) {
    WiFi.begin(wifiSSID, wifiPassword);
    wifiConnectStartMillis = millis();
    wifiState = kConnecting;
  } else if (wifiState == kConnecting && WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP().toString());
    wifiState = kConnected;
  } else if (wifiState == kConnecting) {
    if (millis() - wifiConnectStartMillis > wifiConnectTimeout) {
      Serial.println("Failed to connect to WiFi within the timeout period.");
      WiFi.disconnect();
      wifiState = kDisconnected;
    }
  }
}


void checkWifiConnection() {
  if (wifiState != kConnected) {
    initializeWifiConnection();
  }
}
