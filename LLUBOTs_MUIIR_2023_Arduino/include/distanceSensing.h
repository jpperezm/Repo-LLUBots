#define sensorPin A0          // for the use of a analog sensor

// Interpolation for the IR sensor
const int tableEntries = 10;  // number of entries for the interpolation table
const int mVinterval  = 100;  // value of the intervals in milivolts
const float sensorDistance[tableEntries] = {80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 25.0, 20.0, 15.0, 10.0};


float sensorDistanceRead(float chosenServoAngle, Servo_ESP8266* sensorServo);
float getsensorDistance(int mV);
float seeStraight(Servo_ESP8266* sensorServo);