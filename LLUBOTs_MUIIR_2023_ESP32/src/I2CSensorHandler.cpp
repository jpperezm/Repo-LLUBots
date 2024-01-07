#include <Wire.h>

#include "../include/I2CSensorHandler.h"


void setupI2CSensors() {
  // Inicializa los sensores I2C aquí
  // Ejemplo:
  // Wire.begin();
  // Wire.setClock(400000);
}


float getLeftSensorData() {
  return 0.0;
}


float getRightSensorData() {
  return 1.0;
}



float getUltrasonicData() {
  // Implementa la comunicación I2C aquí para obtener los datos del sensor
  // Ejemplo:
  // Wire.beginTransmission(sensorAddress);
  // ... Envía y recibe datos ...
  // Wire.endTransmission();
  // return data;
  return 0.0;
}
