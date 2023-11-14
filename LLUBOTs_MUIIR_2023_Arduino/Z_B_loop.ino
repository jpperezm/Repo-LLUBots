#include "src\Client\HTML\S_lowBateryServer.cpp"
#include "src\Movement\Roomba\U_actividad1.cpp"
#include "src\Movement\Chess\V_actividad2.cpp"


void loop() {
  
  // First read the voltage value for the correct functioning of the LLUBot.
  digitalWrite(ledPin, LOW);
  sensorVal=1;//digitalRead(voltagePin);

  // Si NO tenemos suficiente batería:
  if (sensorVal == 0){
    // Turn on the warning LED and show the Low Battery Server:
    digitalWrite(ledPin, HIGH);
    lowBatteryServer();
  }
  
  // Si SÍ tenemos suficiente batería:
  else {
    delay(100);
    switch (actividadSel){
      case 1:
        Serial.println("Hey, entré en la actividad 1");
        juegoRoomba();
        Serial.println("Hey, salí de la actividad 1");
        break;
      case 2:
        Serial.println("Hey, entré en la actividad 2");
        maths();
        Serial.println("Hey, salí de la actividad 2");
        break;
      default:
        Serial.println("Hey, estoy en el menú principal");
        principalMenu();
        Serial.println("Hey, me voy del menú principal");
        break;
    }
  }
}  // FINITE INCANTATEM
