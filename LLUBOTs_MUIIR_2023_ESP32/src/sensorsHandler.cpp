#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Arduino.h>

#include "../include/sensorsHandler.h"


// Pin configuration for various sensors

const int MFRC522_RESET_PIN = 0;
const int MFRC522_SS_PIN = 5;
const int LED_PIN = 13;
const int RIGHT_IR_PIN = 27;
const int LEFT_IR_PIN = 14;
const int ULTRASONIC_TRIG_PIN = 25;
const int ULTRASONIC_ECHO_PIN = 26;


// Variables to store the current sensor values

bool rightIRSensorValue;
bool leftIRSensorValue;
int ultrasonicSensorValue;
int NFCSensorValue = -1;

// Initialize MFRC522 (RFID reader) instance
MFRC522 rfidReader(MFRC522_SS_PIN, MFRC522_RESET_PIN);
MFRC522::MIFARE_Key rfidKey;  // Key for RFID operations


// Initializes all the sensors used by the LLUBot.
void initializeSensors() {
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  SPI.begin();
  rfidReader.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    rfidKey.keyByte[i] = 0xFF;
  }
}


// Reads and returns the current value of the right IR sensor.
bool readRightIRSensor() {
  return digitalRead(RIGHT_IR_PIN);
}


// Reads and returns the current value of the left IR sensor.
bool readLeftIRSensor() {
  return digitalRead(LEFT_IR_PIN);
}


// Reads and returns the distance measured by the ultrasonic sensor.
int readUltrasonicSensor() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  int distance = (duration * 0.034) / 2;
  if (distance > 254) distance = 255;
  return distance;
}


// Reads and returns the current value from the NFC sensor.
int readNFCSensor() {
  if (!rfidReader.PICC_IsNewCardPresent() || !rfidReader.PICC_ReadCardSerial()) {
    return NFCSensorValue;
  }

  byte blockAddr = 4;
  byte buffer[18];
  byte size = sizeof(buffer);
  rfidReader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &rfidKey, &(rfidReader.uid));
  rfidReader.MIFARE_Read(blockAddr, buffer, &size);

  rfidReader.PICC_HaltA();
  rfidReader.PCD_StopCrypto1();

  NFCSensorValue = buffer[0];

  return NFCSensorValue;
}


// Updates the stored values of all sensors.
void updateSensors() {
  rightIRSensorValue = readRightIRSensor();
  leftIRSensorValue = readLeftIRSensor();
  ultrasonicSensorValue = readUltrasonicSensor();
  NFCSensorValue = readNFCSensor();

  Serial.print(rightIRSensorValue);
  Serial.print(",");
  Serial.print(leftIRSensorValue);
  Serial.print(",");
  Serial.print(ultrasonicSensorValue);
  Serial.print(",");
  Serial.println(NFCSensorValue);
}


// Returns the last read value from the NFC sensor.
int getNFCSensorValue() {
  return NFCSensorValue;
}


// Returns the last measured distance by the ultrasonic sensor.
int getUltrasonicDistance() {
  return ultrasonicSensorValue;
}


// Returns the last read value from the right IR sensor.
bool getRightIRValue() {
  return rightIRSensorValue;
}


// Returns the last read value from the left IR sensor.
bool getLeftIRValue() {
  return leftIRSensorValue;
}


