#include <Arduino.h>
#include <ArduinoJson.h>

#include "../include/robotStatusCollector.h"
#include "../include/robotStateMachine.h"
#include "../include/sensorsHandler.h"

int leftLineFollowerData;
int rightLineFollowerData;
float ultrasonicData;


void updateSensorData() {
  collectLineFollowerData();
  collectUltrasonicData();
  updateRobotState();
}


String serializeRobotState(RobotState state) {
  switch (state) {
    case kIdle: return "Idle";
    case kSearch: return "Search";
    case kAtHomeFirstTry: return "At Home First Try";
    case kFoundHome: return "FoundHome";
    case kReturn: return "Return";
    case kIdleRoundabout: return "IdleRoundabout";
    case kRoundabout: return "Roundabout";
    case kLeaveRoundabout: return "LeaveRoundabout";
    case kGoingHome: return "GoingHome";
    case kAtHome: return "AtHome";
    case kConfig: return "Config";
    default: return "Unknown";
  }
}


String generateRobotStatusJson() {
  DynamicJsonDocument doc(1024);

  doc["leftLineFollower"] = leftLineFollowerData;
  doc["rightLineFollower"] = rightLineFollowerData;
  doc["ultrasonic"] = ultrasonicData;
  doc["currentState"] = serializeRobotState(currentState);

  String jsonData;
  serializeJson(doc, jsonData);
  return jsonData;
}


void collectLineFollowerData() {
  leftLineFollowerData = readLeftIRSensor();
  rightLineFollowerData = readRightIRSensor();
}


void collectUltrasonicData() {
  ultrasonicData = readUltrasonicSensor();
}