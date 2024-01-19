#include <Arduino.h>

#include "../include/robotStateMachine.h"
#include "../include/I2CSensorHandler.h"
#include "../include/sensorsHandler.h"
#include "../include/clientMQTT.h"

RobotState currentState = kConfig;
RobotState previousState = kConfig;

bool startCommandReceived = false;
bool resetCommandReceived = false;
bool configModeActivated = false;
bool LLUBotLocationSent = false;

int numberOfLLUBots = 5;

int homeName = -1;
int initialStreetName = -1;
int goalStreetName = -1;
int numberOfLLUBotsOnRoundabout = 0;

int NFCCurrentValue;

String LLUBotID = "Blanco";

bool lineFollowerTest = false;

bool atHome() {
  return NFCCurrentValue == homeName;
}


bool foundAHome() {
  return NFCCurrentValue != -1;
}


bool foundInitialStreet() {
  return NFCCurrentValue == initialStreetName;
}


bool foundGoalStreet() {
  return NFCCurrentValue == goalStreetName;
}


void updateRobotState() {
  RobotState lastState = currentState;
  switch (currentState) {
    case kConfig:
      if (resetCommandReceived) {
        Serial.println("Changing to state Idle");
        currentState = kIdle;
      } else if (lineFollowerTest) {
        Serial.println("Changing to state LineFollowerTest");
        currentState = kLineFollowerTest;
      }
      break;
    case kLineFollowerTest:
      if (!lineFollowerTest) {
        Serial.println("Changing to state Config");
        currentState = kConfig;
      }
      break;

    case kIdle:
      if (startCommandReceived) {
        Serial.println("Changing to state Search");
        currentState = kSearch;
      }
      break;

    case kSearch:
      if (atHome()) {
        currentState = kAtHomeFirstTry;
      } else if (configModeActivated) {
        Serial.println("Changing to state Config");
        currentState = kConfig;
      } else if (foundAHome()) {
        Serial.println("Changing to state FoundHome");
        currentState = kFoundHome;
      }
      break;

    case kAtHomeFirstTry:
      if (configModeActivated) {
        Serial.println("Changing to state Config");
        currentState = kConfig;
      } else if (resetCommandReceived) {
        Serial.println("Changing to state Idle");
        currentState = kIdle;
      }
      break;

    case kFoundHome:
      if (LLUBotLocationSent) {
        Serial.println("Changing to state Return");
        currentState = kReturn;
      }
      break;

    case kReturn:
      if (foundInitialStreet()) {
        Serial.println("Changing to state IdleRoundabout");
        currentState = kIdleRoundabout;
      }
      break;

    case kIdleRoundabout:
      if (numberOfLLUBotsOnRoundabout == numberOfLLUBots) {
        Serial.println("Changing to state Roundabout");
        currentState = kRoundabout;
      }
      break;

    case kRoundabout:
      if (foundGoalStreet()) {
        Serial.println("Changing to state LeaveRoundabout");
        currentState = kLeaveRoundabout;
      }
      break;

    case kLeaveRoundabout:
      if (readRightIRSensor() && readLeftIRSensor()) {
        Serial.println("Changing to state GoingHome");
        currentState = kGoingHome;
      }
      break;

    case kGoingHome:
      if (atHome()) {
        Serial.println("Changing to state AtHome");
        currentState = kAtHome;
      }
      break;

    default:
      break;
  }
  previousState = lastState;
}


void handlConfigInitial() {
  sendStopCommand();
}


void handleConfigContinuous() {
  homeName = readNFCSensor();
}


void handleLineFollowerTestInitial() {
  sendLineFollowerCommand();
}


void handleIdleInitial() {
  sendStopCommand();
}


void handleIdleContinuous() {
  initialStreetName = readNFCSensor();
}


void handleSearchInitial() {
  sendLineFollowerCommand();
}


void handleSearchContinuous() {
  int nfcReadValue = readNFCSensor();
  NFCCurrentValue = nfcReadValue == initialStreetName ? -1 : nfcReadValue;
}


void handleAtHomeFirstTryInitial() {
  sendStopCommand();
  publishRoundabout("FoundHome");
}


void handleFoundHomeInitial() {
  sendStopCommand();
  publishLLUBotHomeLocation(NFCCurrentValue);
}


void handleReturnInitial() {
  sendTurnCommand();
}


void handleReturnContinuous() {
  NFCCurrentValue = readNFCSensor();
}


void handleIdleRoundaboutInitial() {
  sendStopCommand();
  publishRoundabout("IdleRoundabout");
}


void handleRoundaboutInitial() {
  sendTurnCommand();
}


void handleRoundaboutContinuous() {
  NFCCurrentValue = readNFCSensor();
}


void handleLeaveRoundaboutInitial() {
  sendTurnCommand();
}


void handleGoingHomeInitial() {
  sendLineFollowerCommand();
}


void handleGoingHomeContinuous() {
  NFCCurrentValue = readNFCSensor();
}


void handleAtHomeInitial() {
  sendStopCommand();
}


void handleRobotState() {
  if (currentState != previousState) {
    switch (currentState) {
      case kConfig: handlConfigInitial(); 
        Serial.println("State Config Initial");
      break;
      case kLineFollowerTest: handleLineFollowerTestInitial(); 
        Serial.println("State LineFollowerTest Initial");
        break;
      case kIdle: handleIdleInitial();  
        Serial.println("State Idle Initial");
      break;
      case kSearch: handleSearchInitial(); 
        Serial.println("State Search Initial");
      break;
      case kAtHomeFirstTry: handleAtHomeFirstTryInitial(); 
        Serial.println("State AtHomeFirstTry Initial");
      break;
      case kFoundHome: handleFoundHomeInitial(); 
        Serial.println("State FoundHome Initial");
      break;
      case kReturn: handleReturnInitial(); 
        Serial.println("State Return Initial");
      break;
      case kIdleRoundabout: handleIdleRoundaboutInitial(); 
        Serial.println("State IdleRoundabout Initial");
      break;
      case kRoundabout: handleRoundaboutInitial(); 
        Serial.println("State Roundabout Initial");
      break;
      case kLeaveRoundabout: handleLeaveRoundaboutInitial(); 
        Serial.println("State LeaveRoundabout Initial");
      break;
      case kGoingHome: handleGoingHomeInitial(); 
        Serial.println("State GoingHome Initial");
      break;
      case kAtHome: handleAtHomeInitial(); 
        Serial.println("State AtHome Initial");
      break;
      default: break;
    }
  }
  switch (currentState) {
    case kConfig: handleConfigContinuous(); 
    break;
    case kLineFollowerTest: break;
    case kIdle: handleIdleContinuous(); break;
    case kSearch: handleSearchContinuous(); break;
    case kAtHomeFirstTry: break;
    case kFoundHome: break;
    case kReturn: handleReturnContinuous(); break;
    case kIdleRoundabout: break;
    case kRoundabout: handleRoundaboutContinuous(); break;
    case kLeaveRoundabout: break;
    case kGoingHome: handleGoingHomeContinuous(); break;
    case kAtHome: break;
    default: break;
  }
}