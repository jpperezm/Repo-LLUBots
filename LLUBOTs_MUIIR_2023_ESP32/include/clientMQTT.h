enum MQTTConnectionState {
  kMQTTDisconnected,
  kMQTTConnecting,
  kMQTTConnected
};

void initializeMQTTConnection();
void establishMQTTConnection();
void checkMQTTConnection();
void handleMQTTLoop();
void publishRobotStatus();
void publishRoundabout(String status);
void publishLLUBotHomeLocation(int homeID);