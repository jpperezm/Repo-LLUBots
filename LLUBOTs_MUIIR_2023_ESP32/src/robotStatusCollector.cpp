#include <Arduino.h>
#include <ArduinoJson.h>

#include "../include/robotStatusCollector.h"
#include "../include/robotStateMachine.h"
#include "../include/sensorsHandler.h"


String serializeRobotState(RobotState state) {
  switch (state) {
    case kIdle: return "Idle";
    case kLineFollowerTest: return "LineFollowerTest";
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
    case kEmergencyStop: return "EmergencyStop";
    default: return "Unknown";
  }
}


String generateRobotStatusJson() {
  DynamicJsonDocument doc(1024);

  doc["LLUBotID"] = LLUBotID;
  doc["leftLineFollower"] = getLeftIRValue();
  doc["rightLineFollower"] = getRightIRValue();
  doc["ultrasonic"] = getUltrasonicDistance();
  doc["NFC"] = getNFCSensorValue();
  doc["currentState"] = serializeRobotState(currentState);
  doc["homeName"] = homeName;
  doc["initialStreetName"] = initialStreetName;
  doc["goalStreetName"] = goalStreetName;
  doc["numberOfLLUBotsOnRoundabout"] = numberOfLLUBotsOnRoundabout;

  String jsonData;
  serializeJson(doc, jsonData);
  return jsonData;
}