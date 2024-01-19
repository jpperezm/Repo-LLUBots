#include <Arduino.h>

#include "../include/robotStateMachine.h"
#include "../include/I2CSensorHandler.h"
#include "../include/sensorsHandler.h"
#include "../include/clientMQTT.h"

RobotState currentState = kConfig;
RobotState previousState = kConfig;
RobotState previousStateEmergencyStop = kConfig;

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
bool emergencyStop = false;


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


bool nonEmergencyStates() {
  return (currentState == kIdleRoundabout) || 
         (currentState == kAtHomeFirstTry);
}


void updateRobotState() {
  RobotState lastState = currentState;

  if (emergencyStop && currentState != kEmergencyStop && !nonEmergencyStates()) {
    previousStateEmergencyStop = lastState;
    currentState = kEmergencyStop;
  }

  if (configModeActivated && currentState != kConfig) {
    configModeActivated = false;
    currentState = kConfig;
  }

  switch (currentState) {
    case kConfig:
      if (resetCommandReceived) {
        Serial.println("Changing to state Idle");
        MQTTdisconnect();
        currentState = kIdle;
        resetCommandReceived = false;
      } else if (lineFollowerTest) {
        Serial.println("Changing to state LineFollowerTest");
        currentState = kLineFollowerTest;
      } 
      break;
    case kLineFollowerTest:
      if (!lineFollowerTest) {
        currentState = kConfig;
        Serial.println("Changing to state kConfig");
      }
      break;

    case kIdle:
      if (startCommandReceived) {
        currentState = kSearch;
         Serial.println("Changing to state kSearch");
         startCommandReceived = false;
      }
      break;

    case kSearch:
      if (atHome()) {
        currentState = kAtHomeFirstTry;
         Serial.println("Changing to state kAtHomeFirstTry");
      } else if (foundAHome()) {
        currentState = kFoundHome;
         Serial.println("Changing to state kFoundHome");
      }
      break;

    case kAtHomeFirstTry:
      if (resetCommandReceived) {
         Serial.println("Changing to state kIdle");
        currentState = kIdle;
        resetCommandReceived = false;
      }
      break;

    case kFoundHome:
      if (LLUBotLocationSent) {
        Serial.println("Changing to state kReturn");
        currentState = kReturn;
        LLUBotLocationSent = false;
      }
      break;

    case kReturn:
      if (foundInitialStreet()) {
        Serial.println("Changing to state kIdleRoundabout");
        currentState = kIdleRoundabout;
      }
      break;

    case kIdleRoundabout:
      if (numberOfLLUBotsOnRoundabout == numberOfLLUBots) {
        Serial.println("Changing to state kRoundabout");
        currentState = kRoundabout;
        numberOfLLUBotsOnRoundabout = 0;
      }
      break;

    case kRoundabout:
      if (foundGoalStreet()) {
        Serial.println("Changing to state kLeaveRoundabout");
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
        Serial.println("Changing to state kAtHome");
        currentState = kAtHome;
      }
      break;
    case kEmergencyStop:
      if (!emergencyStop) {
        currentState = previousStateEmergencyStop;
      }
    default:
      break;
  }
  previousState = lastState;
}


void handleEmergencyStopInitial() {
  sendStopCommand();
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


void handleAtHomeFirstTryContinuous() {
  NFCCurrentValue = readNFCSensor();
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
      case kConfig: handlConfigInitial(); break;
      case kLineFollowerTest: handleLineFollowerTestInitial(); break;
      case kIdle: handleIdleInitial(); break;
      case kSearch: handleSearchInitial(); break;
      case kAtHomeFirstTry: handleAtHomeFirstTryInitial(); break;
      case kFoundHome: handleFoundHomeInitial(); break;
      case kReturn: handleReturnInitial(); break;
      case kIdleRoundabout: handleIdleRoundaboutInitial(); break;
      case kRoundabout: handleRoundaboutInitial(); break;
      case kLeaveRoundabout: handleLeaveRoundaboutInitial(); break;
      case kGoingHome: handleGoingHomeInitial(); break;
      case kAtHome: handleAtHomeInitial(); break;
      case kEmergencyStop: handleEmergencyStopInitial(); break;
      default: break;
    }
  }
  switch (currentState) {
    case kConfig: handleConfigContinuous(); 
    break;
    case kLineFollowerTest: break;
    case kIdle: handleIdleContinuous(); break;
    case kSearch: handleSearchContinuous(); break;
    case kAtHomeFirstTry: handleAtHomeFirstTryContinuous(); break;
    case kFoundHome: break;
    case kReturn: handleReturnContinuous(); break;
    case kIdleRoundabout: break;
    case kRoundabout: handleRoundaboutContinuous(); break;
    case kLeaveRoundabout: break;
    case kGoingHome: handleGoingHomeContinuous(); break;
    case kAtHome: break;
    case kEmergencyStop: break;
    default: break;
  }
}