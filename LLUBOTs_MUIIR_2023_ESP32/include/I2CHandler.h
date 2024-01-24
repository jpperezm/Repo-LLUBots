/**
 * @brief Initializes the I2C communication protocol for interfacing with a slave device that controls the motors. 
 * This sets up necessary configurations for transmitting sensor data to the slave.
 */
void initializeI2C();

/**
 * @brief Sends a command via I2C to the slave to activate the line follower mode. 
 * This instructs the motor controller to use sensor data for line following decisions.
 */
void sendLineFollowerCommand();

/**
 * @brief Sends a stop command via I2C to the slave, instructing it to halt the movement of the motors.
 */
void sendStopCommand();

/**
 * @brief Sends a turn command via I2C to the slave, directing it to execute a turning maneuver based on sensor input.
 */
void sendTurnCommand();

/**
 * @brief Sends the data from the left IR sensor to the slave via I2C. 
 * This data is used to determine if the bot needs to adjust its course while following a line.
 * @param data The intensity reading from the left IR sensor, indicating line presence or absence.
 */
void sendLeftSensorData(uint8_t data);  // TODO: Change the name of this function to sendLeftIRSensorData

/**
 * @brief Sends the data from the right IR sensor to the slave via I2C.
 * This data helps the motor controller in making decisions about steering and navigation while following a line.
 * @param data The intensity reading from the right IR sensor, indicating line presence or absence.
 */
void sendRightSensorData(uint8_t data);  // TODO: Change the name of this function to sendRightIRSensorData
