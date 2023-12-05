void movement (char way, float steps, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances);
void goStraight (char way, float cm, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances);
void turn (char way, float angle, AF_Stepper& motorR, AF_Stepper& motorL, int& next_order, char* orders, int* distances);
