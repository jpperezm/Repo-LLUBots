const int sensorPin = A0; // for the use of a analog sensor

// Interpolation for the IR sensor
const int tableEntries = 10; // number of entries for the interpolation table
const int mVinterval  = 100; // value of the intervals in milivolts
const float sensorDistance[tableEntries] = {80.0, 70.0, 60.0, 50.0, 40.0, 30.0, 25.0, 20.0, 15.0, 10.0};

extern Servo_ESP8266 sensorServo; // Sensor motor (servo)

// initialize the sensorServo object
void initSensorServo(uint8_t servoPin, uint8_t servoAngle);
float sensorDistanceRead(float chosenServoAngle);
float getsensorDistance(int mV);
float seeStraight();