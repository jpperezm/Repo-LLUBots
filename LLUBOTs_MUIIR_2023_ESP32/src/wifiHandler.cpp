#include <WiFi.h>

const char* wifiSSID = "Redmi Note 12 Pro 5G";
const char* wifiPassword = "sonawifi";

unsigned long wifiConnectStartMillis = 0;
const unsigned long wifiConnectTimeout = 10000;


void attemptWifiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    WiFi.begin(wifiSSID, wifiPassword);
    wifiConnectStartMillis = millis();
  } else {
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP().toString());
  }
}


void initializeWifiConnection() {
  attemptWifiConnection();
}


void checkWifiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifiConnectStartMillis > wifiConnectTimeout) {
      WiFi.disconnect();
      attemptWifiConnection();
    }
  }
}
