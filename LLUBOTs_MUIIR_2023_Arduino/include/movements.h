extern int next_order;
extern bool movementRequest;
const int n_orders = 100;  //Maximum number of oreders that the LLUBot may execute
extern char orders[100];  //Orders executed
extern int distances[100];  //Distances moved in each order
extern int next_order;  //the next order and distance to fill

// Physical data of the LLUBot:
const float wheelRadius = 3.2;      // in cm, check this distance with the physical LLUBot
const float wheelsAxisDis = 16.0;   // in cm, check this distance with the physical LLUBot
const int stepperResolution = 256;  // 8 bits
const int stepToMicrostep = 8;      // 8 microsteps are 1 step within the stepper structure
const float twoPi = 2 * 3.1416;     // geometry use of 2 * pi in the perimeter of a circle


void initMotors(float speed);
void movement(char way);
void goStraight(char way, float cm);
void turn (char way, float angle);