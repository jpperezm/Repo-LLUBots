const int servoPin = D7;
const int IRSensorOnePin = A0;

extern int analog;
extern int lecturaSensorIzq;
extern int lecturaSensorDer;

extern bool movementRequest;

void lecturaSensorIR();
void sigueLineas(int lecturaSensorIzq, int lecturaSensorDer);