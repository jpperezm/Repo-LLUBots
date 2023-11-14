// FUNCTION 8: CHOOSE THE CORRECT WAY: -----------------------------------------------------------------------------------//
void chooseWay() {

  // First the variables are created:
  float minCm = 21.0; // 20 cm is the minimum distance to run, as it's the larger distance of the LLUBot
  float readCm = 0;   // for the reading of the distance
  float readAngle = 90.0; // which represents: the read angle is 90, looking for a better angle to run

  // A 45 degree reading is taken from the current position of the LLUBot to its right:
  for(int i=0; i<45; i++) {  // 
    readCm = sensorDistanceRead(i);
    if (readCm >= minCm){
      minCm = readCm;
      readAngle = i;
    }
  }

  // Another 45 degree reading is taken from the current position of the LLUBot to its left:
  for(int i=135; i<181; i++) {  // 
    readCm = sensorDistanceRead(i);
    if (readCm >= minCm){
      minCm = readCm;
      readAngle = i;
    }
  }

  // To check the correct functioning:
  Serial.print("Angle to turn: ");
  Serial.println(readAngle);
  Serial.print("Distance to go: ");
  Serial.println(minCm);
  

  // It is assessed whether there are possible ways to advance the robot completely
  if (minCm > 21.0) {

    // To return the sensor to its original way:
    sensorServo.write(90.0); // 90 degrees --> front
    delay(50); // servo movement delay

    if (readAngle < 90.0) {
      turn('R', 90.0 - readAngle);
    }
    else {
      turn('L', readAngle - 90.0);
    }
    goStraight('F',minCm);
  } 
  
  // If there are no tracks, turn 180 degrees to look at the other alternatives
  else {
    turn('L',180); 

    // It evaluates once again
    for (int i=0; i<181; i++) { // the other 180 degrees
      readCm = sensorDistanceRead(i);
      if (readCm >= minCm) {
        minCm = readCm;
        readAngle = i;
      }
    }

    if (minCm > 21.0) {

      // To return the sensor to its original way:
      sensorServo.write(90.0); // 90 degrees --> front
      delay(50); // servo movement delay

      if (readAngle < 90.0) {
        turn('R', 90.0 - readAngle);
      }
      else {
        turn('L', readAngle - 90.0);
      }
      goStraight('F',minCm);
    }
    else {
      /* Mostrar el error en pantalla, tal como "El LLUBot no se puede mover"*/
      Serial.println("No me puedo mover");
    }
  }
} //----------------------------------------------------------------------------------------------------------------------//

// FUNCTION 9: ROOMBA TIME: ----------------------------------------------------------------------------------------------//
void roombaTime(int n){
  int n_ = n;
  next_order=0;//the next order and distance to fill
  Serial.print("Within RoombaTime ");
  Serial.println(n_);

  for (int i=0;i<n_;i++) {
    Serial.print("Round number: ");
    Serial.println(i);
    int advanceCm = seeStraight()  ;//aki avía un menos -
    Serial.println(advanceCm);
    
    if (advanceCm > 21.0) {
      goStraight('F', advanceCm);
    }
    else {
      chooseWay();
    }
  }
  
  for(int i=0;i<next_order;i++){
    Serial.print("La ");
    Serial.print(i);
    Serial.print(" ª orden ejecutada fue ");
    Serial.print(orders[i]);
    Serial.print(" y recorrió una distancia de ");
    Serial.print(distances[i]);
    if(orders[i]=='F'||orders[i]=='B'){
      Serial.println(" cm");
    }
    else{
      Serial.println(" grados");
    }
  }
} 