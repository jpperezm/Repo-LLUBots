#include <Arduino.h>
#include <Wire.h>

#include "../include/I2CSensorHandler.h"


void initializeI2CSensors() {
  Wire.begin();
}


void sendCommand(char command) {
  // m, p, g, n, i, d
  Wire.beginTransmission(8);
  Wire.write(command);
  Wire.endTransmission();
}


void sendFollowLineCommand() {
  sendCommand('m');
}


void sendStopCommand() {
  sendCommand('p');
}


void sendTurn180Command() {
  sendCommand('g');
}


void sendTurn90Command() {
  sendCommand('n');
}


void sendLeftSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('i');
  Wire.write(data);
  Wire.endTransmission();
}


void sendRightSensorData(uint8_t data) {
  Wire.beginTransmission(8);
  Wire.write('d');
  Wire.write(data);
  Wire.endTransmission();
}