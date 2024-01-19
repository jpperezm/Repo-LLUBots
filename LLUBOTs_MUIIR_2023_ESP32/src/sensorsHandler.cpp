#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Arduino.h>

#include "../include/sensorsHandler.h"

const int RESET_PIN = 0;
const int SS_PIN = 5;
const int LED_PIN = 13;
const int RIGHT_IR_PIN = 27; // Right Infrared Sensor
const int LEFT_IR_PIN = 14;  // Left Infrared Sensor
const int TRIG_PIN = 25;     // Ultrasonic Trigger
const int ECHO_PIN = 26;     // Ultrasonic Echo

bool rightIRSensorValue;
bool leftIRSensorValue;
int ultrasonicSensorValue;
int NFCSensorValue = -1;

MFRC522 mfrc522(SS_PIN, RESET_PIN);
MFRC522::MIFARE_Key key;


void initializeSensors() {
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}


bool readRightIRSensor() {
  return digitalRead(RIGHT_IR_PIN);
}


bool readLeftIRSensor() {
  return digitalRead(LEFT_IR_PIN);
}


int readUltrasonicSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = (duration * 0.034) / 2;
  if (distance > 254) distance = 255;
  return distance;
}


int readNFCSensor() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return NFCSensorValue;
  }

  byte blockAddr = 4;
  byte buffer[18];
  byte size = sizeof(buffer);
  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));
  mfrc522.MIFARE_Read(blockAddr, buffer, &size);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  NFCSensorValue = buffer[0];

  return NFCSensorValue;
}


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


int getNFCSensorValue() {
  return NFCSensorValue;
}


int getUltrasonicDistance() {
  return ultrasonicSensorValue;
}


bool getRightIRValue() {
  return rightIRSensorValue;
}


bool getLeftIRValue() {
  return leftIRSensorValue;
}


