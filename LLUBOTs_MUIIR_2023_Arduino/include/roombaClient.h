class AF_Stepper;

void juegoRoomba(WiFiServer* server, Servo_ESP8266* sensorServo, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances, int& sensorVal, int& nextActivity, int& actividadSel);