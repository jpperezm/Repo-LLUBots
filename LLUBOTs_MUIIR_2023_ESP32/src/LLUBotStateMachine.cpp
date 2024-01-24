#include <Arduino.h>

#include "../include/LLUBotStateMachine.h"
#include "../include/I2CHandler.h"
#include "../include/sensorsHandler.h"
#include "../include/clientMQTT.h"

// Global variables to manage LLUBot's state and sensor data.

LLUBotState currentState = kConfig;
LLUBotState previousState = kConfig;
LLUBotState previousStateEmergencyStop = kConfig;

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


// Functions to check LLUBot's position relative to NFC sensor readings.

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


// Updates the state of the LLUBot based on sensor inputs and commands.
void updateRobotState() {
  LLUBotState lastState = currentState;

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
        MQTTdisconnect();
        currentState = kIdle;
        resetCommandReceived = false;
      } else if (lineFollowerTest) {
        currentState = kLineFollowerTest;
      } 
      break;
    case kLineFollowerTest:
      if (!lineFollowerTest) {
        currentState = kConfig;
      }
      break;

    case kIdle:
      if (startCommandReceived) {
        currentState = kSearch;
        startCommandReceived = false;
      }
      break;

    case kSearch:
      if (atHome()) {
        currentState = kAtHomeFirstTry;
      } else if (foundAHome()) {
        currentState = kFoundHome;
      }
      break;

    case kAtHomeFirstTry:
      if (resetCommandReceived) {
        currentState = kIdle;
        resetCommandReceived = false;
      }
      break;

    case kFoundHome:
      if (LLUBotLocationSent) {
        currentState = kReturn;
        LLUBotLocationSent = false;
      }
      break;

    case kReturn:
      if (foundInitialStreet()) {
        currentState = kIdleRoundabout;
      }
      break;

    case kIdleRoundabout:
      if (numberOfLLUBotsOnRoundabout == numberOfLLUBots) {
        currentState = kRoundabout;
        numberOfLLUBotsOnRoundabout = 0;
      }
      break;

    case kRoundabout:
      if (foundGoalStreet()) {
        currentState = kLeaveRoundabout;
      }
      break;

    case kLeaveRoundabout:
      if (readRightIRSensor() && readLeftIRSensor()) {
        currentState = kGoingHome;
      }
      break;

    case kGoingHome:
      if (atHome()) {
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


// State-specific initial actions when entering a new state.

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
  int homeFound = NFCCurrentValue;
  publishEntryStreetToLLUBotForHome(homeFound);
}


void handleReturnInitial() {
  sendTurnCommand();
}


void handleReturnContinuous() {
  NFCCurrentValue = readNFCSensor();
}


void handleIdleRoundaboutInitial() {
  sendStopCommand();
  publishRoundabout("ArrivedAtRoundabout");
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


// Continuously executed actions within the current state.
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