enum RobotState {
  kIdle, 
  kSearch,
  kHome, 
  kFoundHome, 
  kReturn,
  kIdleRoundabout,
  kRoundabout,
  kLeaveRoundabout,
  kGoingHome,
  kConfig
};

extern bool ready;
extern bool startCommandReceived;
extern bool foundHouse;
extern bool isAtHome;
extern bool goBack;
extern bool atRoundabout;
extern bool allReady;
extern bool foundWayHome;
extern bool goHome;
extern bool configModeActivated;

void updateSensorData();
String generateRobotStatusJson();
void collectLineFollowerData();
void collectUltrasonicData();
extern void updateRobotState();