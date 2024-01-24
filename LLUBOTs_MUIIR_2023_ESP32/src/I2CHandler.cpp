#include <Arduino.h>
#include <Wire.h>

#include "../include/I2CHandler.h"

// Initializes I2C communication.
void initializeI2C() {
  Wire.begin();
}

// Sends a command over I2C to the slave device.
void sendI2CCommand(char command) {
  Wire.beginTransmission(8); // 8 is the I2C address of the slave device
  Wire.write(command);
  Wire.endTransmission();
}

// Commands the line follower to start via I2C.
void sendLineFollowerCommand() {
  sendI2CCommand('g'); // 'g' is the command for 'go' or start line following
}

// Sends a stop command to the motors via I2C.
void sendStopCommand() {
  sendI2CCommand('s'); // 's' is the command for stop
}

// Sends a turn command to the motors via I2C.
void sendTurnCommand() {
  sendI2CCommand('t'); // 't' is the command for turn
}

// Sends data from the left IR sensor to the slave device via I2C.
void sendLeftSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('l'); // 'l' indicates data from left sensor
  Wire.write(data);
  Wire.endTransmission();
}

// Sends data from the right IR sensor to the slave device via I2C.
void sendRightSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('r'); // 'r' indicates data from right sensor
  Wire.write(data);
  Wire.endTransmission();
}
