// Initialize the Wi-Fi client:
extern WiFiClient wifiClient;

// Initialize the MQTT client:
extern PubSubClient MQTTClient;

// Initialize the MQTT client:
extern const char *mqtt_broker;
extern const char *movement_topic;
const int mqtt_port = 1883;

void mqttSetup();
void callback(char *topic, byte *payload, unsigned int length);
void connect();