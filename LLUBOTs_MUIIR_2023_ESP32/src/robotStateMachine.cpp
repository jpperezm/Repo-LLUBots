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

int homeName;
int initialStreetName;
int goalStreetName;
int numberOfLLUBotsOnRoundabout = 0;

int NFCCurrentValue;

bool atHome() {
  return NFCCurrentValue == homeName;
}


bool foundAHome() {
  return NFCCurrentValue != NO_NFC_DETECTED;
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


void handleIdleInitial() {
  sendStopCommand();
}


void handleIdleContinuous() {
  initialStreetName = readNFCSensor();
}


void handleSearchInitial() {
  sendFollowLineCommand();
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
  sendTurn180Command();
  delay(30000);
  sendFollowLineCommand();
}


void handleReturnContinuous() {
  int NFCCurrentValue = readNFCSensor();
}


void handleIdleRoundaboutInitial() {
  sendStopCommand();
  publishRoundabout("IdleRoundabout");
}


void handleRoundaboutInitial() {
  sendTurn90Command();
  delay(15000);
  sendFollowLineCommand();
}


void handleRoundaboutContinuous() {
  int NFCCurrentValue = readNFCSensor();
}


void handleLeaveRoundaboutInitial() {
  sendTurn90Command();
}


void handleGoingHomeInitial() {
  sendFollowLineCommand();
}


void handleGoingHomeContinuous() {
  int NFCCurrentValue = readNFCSensor();
}


void handleAtHomeInitial() {
  sendStopCommand();
}


void handleRobotState() {
  if (currentState != previousState) {
    switch (currentState) {
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
    case kIdle: handleIdleContinuous(); 
      Serial.println("State Idle Continuous");
    break;
    case kSearch: handleSearchContinuous(); 
      Serial.println("State Search Continuous");
    break;
    case kAtHomeFirstTry: 
      Serial.println("State AtHomeFirstTry Continuous");
    break;
    case kFoundHome: 
      Serial.println("State FoundHome Continuous");
    break;
    case kReturn: handleReturnContinuous(); 
      Serial.println("State Return Continuous");
    break;
    case kIdleRoundabout: 
      Serial.println("State IdleRoundabout Continuous");
    break;
    case kRoundabout: handleRoundaboutContinuous(); 
      Serial.println("State Roundabout Continuous");
    break;
    case kLeaveRoundabout: 
      Serial.println("State LeaveRoundabout Continuous");
    break;
    case kGoingHome: handleGoingHomeContinuous(); 
      Serial.println("State GoingHome Continuous");
    break;
    case kAtHome: 
      Serial.println("State AtHome Continuous");
    break;
    default: break;
  }
}