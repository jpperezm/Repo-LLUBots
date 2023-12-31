#include <Arduino.h>

#include "../include/lineFollower.h"
#include "../include/movements.h"

int analog = 0;
int lecturaSensorIzq = 0;
int lecturaSensorDer = 0;
bool movementRequest = false;


void lecturaSensorIR() {
 
  /*
  Wire.beginTransmission(8); //El 8es la dirección i2c del arduino
  Wire.write('d');
  Wire.endTransmission;
  Wire.requestFrom(8,2);

  lecturaSensorDer = wire.read();

  Wire.beginTransmission(8); //El 8es la dirección i2c del arduino
  Wire.write('i');
  Wire.endTransmission;
  Wire.requestFrom(8,2);

  lecturaSensorIzq = wire.read();

  Wire.beginTransmission(8); //El 8es la dirección i2c del arduino
  Wire.write('u');
  Wire.endTransmission;
  Wire.requestFrom(8,2);

  lecturaUltrasonidos = wire.read();
  
  */
  analog = analogRead(IRSensorOnePin); // Almacena la lectura del sensor izquierdo
  lecturaSensorIzq = digitalRead(servoPin); // Almacena la lectura del sensor derecho
  
  //Como estamos usando un sensor de forma analogica, almacenamos el valor y lo establecemos en 1/0 dependiendo un umbral
  if(
    analog > 100)
  {
    lecturaSensorDer = 1; 
  }else{
    lecturaSensorDer = 0;
  } 

}

void sigueLineas(int lecturaSensorIzq, int lecturaSensorDer) {
 
  //lecturaSensorIR(); // Se lee el valor de los sensores IR, esto aparecerá en el loop o en algun otro lado, no aquí
  // 4 options: 'F'orward, 'B'ackward, 'R'ight, 'L'eft

  // Se analiza el resultado de los sensores para hacer que el robot siga la línea negra
 
  // Si el resultado de ambos sensores es 0 (Da media vuelta)
  if(lecturaSensorIzq == 0 && lecturaSensorDer == 0)
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
  if (lecturaSensorIzq == 0 && lecturaSensorDer == 1)
  {
    movementRequest = true;
    movement ('R'); 
    Serial.println("Intento ir a la derecha");
     // El robot gira a la derecha
 
  }
  // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
  if (lecturaSensorIzq == 1 && lecturaSensorDer == 0)
  {
    movementRequest = true;
    movement ('L'); 
    Serial.println("Intento ir a la izquierda");
 
  }
  // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
  if (lecturaSensorIzq == 1 && lecturaSensorDer == 1)
  {
    movementRequest = true;
    movement ('F'); // El robot avanza
    Serial.println("Moviendo hacia adelante");
  }
 
}
//d, i, u, n
void homeDetected() {


  turn ('R', 45); 

  while(lecturaSensorDer != 1)  //Mientras no se detecte que el sensor de la derecha ha alcanzado la linea ...
  {
    lecturaSensorIR();
    turn ('R', 5);              //Gira a la derecha 5 grados
  }
}