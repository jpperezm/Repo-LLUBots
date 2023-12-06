#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Servo_ESP8266.h>
#include <String.h>

#include "../include/roombaClient.h"
#include "../include/mainClient.h"
#include "../include/lowBatteryClient.h"
#include "../include/roomba.h"
#include "../include/movements.h"
#include "../include/serverConfig.h"

void roombaGame() {
  int movA1 = 0;
  int valueA1 = 0;
  while (selectedActivity == 1) {
    int nRunRoomba;
    lowBatteryMock = 1; // TODO Low battery control using batteryVoltagePin

    if (lowBatteryMock == 0) {
      lowBatteryClient();
      continue;
    } 

    WiFiClient client;
    waitForClient(client);
    String request = readClientRequest(client);

    // Match the request
    int pos = request.lastIndexOf('-');
    
    if (request.indexOf("/ROOMBA") != -1) {
      valueA1 = 1;
      request.remove(0, pos + 1);
      movA1 = request.toInt();
      nextActivity = 1;
      }
    if (request.indexOf("/BACK") != -1) {
      nextActivity = 0;
    }
  
    // Web Page
    client.println(roombaPage1);
    showTrace(client);
    client.println(roombaPage2);

    // Roomba movement
    selectedActivity = nextActivity;
    if(valueA1 == 1){
      nRunRoomba = movA1;
      Serial.println(nRunRoomba);
      roombaTime(nRunRoomba);
      valueA1 = 0;
    }

    Serial.println("Current activity: 1");
    Serial.print("Selected activity: ");
    Serial.println(selectedActivity);

    Serial.println("Client disconnected");
    Serial.println();
  }
}