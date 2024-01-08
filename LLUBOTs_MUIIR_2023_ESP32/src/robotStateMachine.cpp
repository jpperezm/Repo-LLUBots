#include <Arduino.h>

#include "../include/robotStateMachine.h"
#include "../include/I2CSensorHandler.h"
#include "../include/sensorsHandler.h"
#include "../include/clientMQTT.h"

RobotState currentState = kIdle;
RobotState previousState = kIdle;

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
        currentState = kIdle;
      }
      break;

    case kIdle:
      if (startCommandReceived) {
        currentState = kSearch;
      }
      break;

    case kSearch:
      if (atHome()) {
        currentState = kAtHomeFirstTry;
      } else if (configModeActivated) {
        currentState = kConfig;
      } else if (foundAHome()) {
        currentState = kFoundHome;
      }
      break;

    case kAtHomeFirstTry:
      if (configModeActivated) {
        currentState = kConfig;
      } else if (resetCommandReceived) {
        currentState = kIdle;
      }
      break;

    case kFoundHome:
      if (LLUBotLocationSent) {
        currentState = kReturn;
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

    default:
      break;
  }
  if (currentState != lastState) {
    previousState = lastState;
  }  
}


void handleIdle() {
  sendStopCommand();
  initialStreetName = readNFCSensor();
}


void handleSearch() {
  sendFollowLineCommand();
  int nfcReadValue = readNFCSensor();
  NFCCurrentValue = nfcReadValue == initialStreetName ? -1 : nfcReadValue;
}


void handleAtHomeFirstTry() {
  sendStopCommand();
  publishRoundabout("FoundHome");
}


void handleFoundHome() {
  sendStopCommand();
  publishLLUBotHomeLocation(NFCCurrentValue);
}


void handleReturn() {
  sendTurn180Command();
  delay(30000);
  sendFollowLineCommand();
  int NFCCurrentValue = readNFCSensor();
}


void handleIdleRoundabout() {
  sendStopCommand();
  publishRoundabout("IdleRoundabout");
}


void handleRoundabout() {
  sendTurn90Command();
  delay(15000);
  sendFollowLineCommand();
  int NFCCurrentValue = readNFCSensor();
}


void handleLeaveRoundabout() {
  sendTurn90Command();
}


void handleGoingHome() {
  sendFollowLineCommand();
  int NFCCurrentValue = readNFCSensor();
}


void handleAtHome() {
  sendStopCommand();
}


void handleRobotState() {
  if (currentState != previousState) {
    switch (currentState) {
      case kIdle: handleIdle(); break;
      case kSearch: handleSearch(); break;
      case kAtHomeFirstTry: handleAtHomeFirstTry(); break;
      case kFoundHome: handleFoundHome(); break;
      case kReturn: handleReturn(); break;
      case kIdleRoundabout: handleIdleRoundabout(); break;
      case kRoundabout: handleRoundabout(); break;
      case kLeaveRoundabout: handleLeaveRoundabout(); break;
      case kGoingHome: handleGoingHome(); break;
      case kAtHome: handleAtHome(); break;
      default: break;
    }
  }
}