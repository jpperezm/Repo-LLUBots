enum MQTTConnectionState {
  kMQTTDisconnected,
  kMQTTConnecting,
  kMQTTConnected
};

extern int LLUBotID;

extern const char *mqtt_broker;
extern const char *home_topic;
extern const char *config_topic;
const int mqtt_port = 1883;

void initializeMQTTConnection();
void establishMQTTConnection();
void checkMQTTConnection();
void handleMQTTLoop();
void callback(char *topic, byte *payload, unsigned int length);
void publishRobotStatus();