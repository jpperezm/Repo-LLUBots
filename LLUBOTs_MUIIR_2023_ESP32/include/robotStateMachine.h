enum RobotState {
  kIdle, 
  kSearch,
  kAtHomeFirstTry, 
  kFoundHome, 
  kReturn,
  kIdleRoundabout,
  kRoundabout,
  kLeaveRoundabout,
  kGoingHome,
  kAtHome,
  kConfig,
  kLineFollowerTest,
  kEmergencyStop
};

extern RobotState currentState;

extern bool startCommandReceived;
extern bool resetCommandReceived;
extern bool configModeActivated;
extern bool LLUBotLocationSent;

extern int numberOfLLUBots;

extern int homeName;
extern int initialStreetName;
extern int goalStreetName;
extern int numberOfLLUBotsOnRoundabout;

extern String LLUBotID;

extern bool lineFollowerTest;
extern bool emergencyStop;

void updateRobotState();
void handleRobotState();