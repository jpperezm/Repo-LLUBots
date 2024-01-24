/**
 * @brief Enumeration representing the state of the MQTT connection.
 * This enum helps in managing and understanding the current MQTT connection status.
 */
enum MQTTConnectionState {
  kMQTTDisconnected,   /**< Indicates that the MQTT connection is currently disconnected. */
  kMQTTConnecting,     /**< Indicates that the MQTT connection is in the process of being established. */
  kMQTTConnected       /**< Indicates that the MQTT connection has been successfully established. */
};

/**
 * @brief Initializes the MQTT connection, setting up necessary parameters and configurations.
 */
void initializeMQTTConnection();

/**
 * @brief Attempts to establish the MQTT connection. This involves connecting to the MQTT broker with appropriate credentials.
 */
void establishMQTTConnection();

/**
 * @brief Checks the status of the MQTT connection and updates the connection state accordingly.
 */
void checkMQTTConnection();

/**
 * @brief Handles the MQTT loop, ensuring continuous communication and message handling with the MQTT broker.
 */
void handleMQTTLoop();

/**
 * @brief Publishes the current status of the robot to a server using MQTT. This status can include various sensor readings and operational states.
 */
void publishRobotStatus();

/**
 * @brief Publishes a message with a specific status to the roundabout topic.
 * @param status The status of the roundabout. It can be "ArrivedAtRoundabout" or "FoundHome".
 *               This is used to coordinate actions and movements of multiple LLUBots at the roundabout.
 */
void publishRoundabout(String status);

/**
 * @brief Publishes the entry street to LLUBot for home.
 * @param homeName The name of the home to which the LLUBot is assigned. This helps the LLUBot in navigating to its designated home.
 */
void publishEntryStreetToLLUBotForHome(int homeName);

/**
 * @brief Disconnects from the MQTT broker, effectively shutting down the MQTT communication.
 */
void MQTTdisconnect();
