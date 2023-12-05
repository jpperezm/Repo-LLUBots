class AF_Stepper;

void chooseWay(Servo_ESP8266* sensorServo, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances);
void roombaTime(int n, Servo_ESP8266* sensorServo, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances);