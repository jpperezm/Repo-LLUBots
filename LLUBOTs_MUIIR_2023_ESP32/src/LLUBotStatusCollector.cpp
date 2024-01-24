#include <Arduino.h>
#include <ArduinoJson.h>

#include "../include/LLUBotStatusCollector.h"
#include "../include/LLUBotStateMachine.h"
#include "../include/sensorsHandler.h"

const int documentSize = 1024;


// Converts the current state of LLUBot into a readable string format
String convertLLUBotStateToString(LLUBotState state) {
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


// Generates a JSON string representing the current status of the LLUBot.
String generateLLUBotStatusJson() {
  DynamicJsonDocument doc(documentSize);

  doc["LLUBotID"] = LLUBotID;
  doc["leftLineFollower"] = getLeftIRValue();
  doc["rightLineFollower"] = getRightIRValue();
  doc["ultrasonic"] = getUltrasonicDistance();
  doc["NFC"] = getNFCSensorValue();
  doc["currentState"] = convertLLUBotStateToString(currentState);
  doc["homeName"] = homeName;
  doc["initialStreetName"] = initialStreetName;
  doc["goalStreetName"] = goalStreetName;
  doc["numberOfLLUBotsOnRoundabout"] = numberOfLLUBotsOnRoundabout;

  String jsonData;
  serializeJson(doc, jsonData);
  return jsonData;
}