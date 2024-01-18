#include <AccelStepper.h>

#include "../include/motorsMovement.h"

const int MOTOR_INTERFACE_TYPE 8;

const int MOTOR_PIN1 = 26;
const int MOTOR_PIN2 = 25;
const int MOTOR_PIN3 = 33;
const int MOTOR_PIN4 = 32;

AccelStepper stepper(MOTOR_INTERFACE_TYPE, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);