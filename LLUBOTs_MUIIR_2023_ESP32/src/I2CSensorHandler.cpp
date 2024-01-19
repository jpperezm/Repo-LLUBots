#include <Arduino.h>
#include <Wire.h>

#include "../include/I2CSensorHandler.h"


void initializeI2CSensors() {
  Wire.begin();
}


void sendCommand(char command) {
  Wire.beginTransmission(8);
  Wire.write(command);
  Wire.endTransmission();
}


void sendLineFollowerCommand() {
  sendCommand('g');
}


void sendStopCommand() {
  sendCommand('s');
}


void sendTurnCommand() {
  sendCommand('t');
}


void sendLeftSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('l');
  Wire.write(data);
  Wire.endTransmission();
}


void sendRightSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('r');
  Wire.write(data);
  Wire.endTransmission();
}