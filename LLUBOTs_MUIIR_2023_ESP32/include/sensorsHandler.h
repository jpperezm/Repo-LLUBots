/**
 * @brief Initializes the sensors used by the LLUBot, including line-following IR sensors, ultrasonic sensor, and NFC sensor.
 * This process may involve setting up initial configurations and calibration procedures.
 */
void initializeSensors();

/**
 * @brief Reads the value of the right IR line-following sensor.
 * @return true if the right IR sensor detects a line, false otherwise.
 */
bool readRightIRSensor();

/**
 * @brief Reads the value of the left IR line-following sensor.
 * @return true if the left IR sensor detects a line, false otherwise.
 */
bool readLeftIRSensor();

/**
 * @brief Reads the distance measured by the ultrasonic sensor.
 * @return The distance to the nearest object in centimeters as measured by the ultrasonic sensor.
 */
int readUltrasonicSensor();

/**
 * @brief Reads the data from the NFC sensor.
 * @return The value or data read from the NFC sensor, typically used for identification or communication purposes.
 */
int readNFCSensor();

/**
 * @brief Retrieves the latest value read from the NFC sensor.
 * @return The most recent value obtained from the NFC sensor.
 */
int getNFCSensorValue();

/**
 * @brief Retrieves the latest distance measured by the ultrasonic sensor.
 * @return The most recent distance measurement in centimeters as detected by the ultrasonic sensor.
 */
int getUltrasonicDistance();

/**
 * @brief Retrieves the latest reading from the right IR line-following sensor.
 * @return true if the most recent reading of the right IR sensor detected a line, false otherwise.
 */
bool getRightIRValue();

/**
 * @brief Retrieves the latest reading from the left IR line-following sensor.
 * @return true if the most recent reading of the left IR sensor detected a line, false otherwise.
 */
bool getLeftIRValue();

/**
 * @brief Updates the sensor readings by acquiring the latest data from all sensors, including line-following IR sensors.
 * This function should be called regularly to ensure up-to-date sensor information.
 */
void updateSensors();
