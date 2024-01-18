#include <Arduino.h>

#include "../include/lineFollower.h"
#include "../include/movements.h"

int leftIRSensorRead = 0;
int rightIRSensorRead = 0;
bool movementRequest = false;

void lineFollower(int leftIRSensorRead, int rightIRSensorRead) {
 
  //lecturaSensorIR(); // Se lee el valor de los sensores IR, esto aparecerá en el loop o en algun otro lado, no aquí
  // 4 options: 'F'orward, 'B'ackward, 'R'ight, 'L'eft

  // Se analiza el resultado de los sensores para hacer que el robot siga la línea negra
 
  // Si el resultado de ambos sensores es 0 (Da media vuelta)
  if(leftIRSensorRead == 0 && rightIRSensorRead == 0)
  {
    //Serial.println("Esta no es mi casa");
    //homeDetected();
    if (movementRequest){
      movement ('R'); 
      Serial.println("Intento ir a la derecha");
    }
    if (movementRequest){
      movement ('L'); 
      Serial.println("Intento ir a la izquierda");
    }
  }
  // Si el izquierdo retorna 0 (zona blanca) y el derecho 1 (negra) el robot gira derecha
  if (leftIRSensorRead == 0 && rightIRSensorRead == 1)
  {
    movementRequest = true;
    movement ('R'); 
    Serial.println("Intento ir a la derecha");
     // El robot gira a la derecha
 
  }
  // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
  if (leftIRSensorRead == 1 && rightIRSensorRead == 0)
  {
    movementRequest = true;
    movement ('L'); 
    Serial.println("Intento ir a la izquierda");
 
  }
  // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
  if (leftIRSensorRead == 1 && rightIRSensorRead == 1)
  {
    movementRequest = true;
    movement ('F'); // El robot avanza
    Serial.println("Moviendo hacia adelante");
  }
 
}
//d, i, u, n
void homeDetected() {


  turn ('R', 45); 

  while(rightIRSensorRead != 1)  //Mientras no se detecte que el sensor de la derecha ha alcanzado la linea ...
  {
    lecturaSensorIR();
    turn ('R', 5);              //Gira a la derecha 5 grados
  }
}