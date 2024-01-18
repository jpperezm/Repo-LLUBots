enum MQTTConnectionState {
  kMQTTDisconnected,
  kMQTTConnecting,
  kMQTTConnected
};

extern int LLUBotID;

void initializeMQTTConnection();
void establishMQTTConnection();
void checkMQTTConnection();
void handleMQTTLoop();
void publishRobotStatus();
void publishRoundabout(String status);
void publishLLUBotHomeLocation(int homeID);