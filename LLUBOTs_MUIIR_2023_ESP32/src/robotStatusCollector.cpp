#include <Arduino.h>
#include <ArduinoJson.h>

#include "../include/robotStatusCollector.h"
#include "../include/I2CSensorHandler.h"

int leftLineFollowerData;
int rightLineFollowerData;
float ultrasonicData;
RobotState currentState = kIdle;

bool ready = false;
bool startCommandReceived = false;
bool foundHouse = false;
bool isAtHome = false;
bool goBack = false;
bool atRoundabout = false;
bool allReady = false;
bool foundWayHome = false;
bool goHome = false;
bool configModeActivated = false;


void updateSensorData() {
  collectLineFollowerData();
  collectUltrasonicData();
  updateRobotState();
}


String serializeRobotState(RobotState state) {
  switch (state) {
    case kIdle: return "Idle";
    case kSearch: return "Search";
    case kHome: return "Home";
    case kFoundHome: return "FoundHome";
    case kReturn: return "Return";
    case kIdleRoundabout: return "IdleRoundabout";
    case kRoundabout: return "Roundabout";
    case kLeaveRoundabout: return "LeaveRoundabout";
    case kGoingHome: return "GoingHome";
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
  leftLineFollowerData = getLeftSensorData();
  rightLineFollowerData = getRightSensorData();
}


void collectUltrasonicData() {
  ultrasonicData = getUltrasonicData();
}


void updateRobotState() {
  switch (currentState) {
    case kConfig:
      if (ready) {
        currentState = kSearch;
      }
      break;

    case kIdle:
      if (startCommandReceived) {
        currentState = kSearch;
      }
      break;

    case kSearch:
      if (foundHouse && isAtHome) {
        currentState = kHome;
      }
      else if (foundHouse && !isAtHome) {
        currentState = kFoundHome;
      }
      else if (configModeActivated) {
        currentState = kConfig;
      }
      break;

    case kHome:
      if (configModeActivated) {
        currentState = kConfig;
      }
      break;

    case kFoundHome:
      if (goBack) {
        currentState = kReturn;
      }
      break;

    case kReturn:
      if (atRoundabout) {
        currentState = kIdleRoundabout;
      }
      break;

    case kIdleRoundabout:
      if (allReady) {
        currentState = kRoundabout;
      }
      break;

    case kRoundabout:
      if (foundWayHome) {
        currentState = kLeaveRoundabout;
      }
      break;

    case kLeaveRoundabout:
      if (goHome) {
        currentState = kGoingHome;
      }
      break;
    
    case kGoingHome:
      if (isAtHome) {
        currentState = kHome;
      }
      break;

    default:
      break;
  }
}