#include <Arduino.h>
#include <String.h>
#include <ESP8266WiFi.h>
#include <DMotor_mod.h>

#include "../include/mainClient.h"
#include "../include/movements.h"
#include "../include/serverConfig.h"

int selectedActivity = 0;
int nextActivity = 0;

void mainMenu() {
  while (selectedActivity == 0) {
    WiFiClient client;
    waitForClient(client);
    String request = readClientRequest(client);
   
    // Match the request
    int pos = request.lastIndexOf('-');
    next_order = 0;

    if (request.indexOf("/A1") != -1) {
      nextActivity = 1;
      }
    if (request.indexOf("/A2") != -1) {
      nextActivity = 2;
    }
    if (request.indexOf("/BACK") != -1) {
      nextActivity = 0;
    }

    // Web Page
    client.println(mainPage);

    selectedActivity = nextActivity;
    Serial.println("Actividad actual: 0");
    Serial.print("Actividad seleccionada: ");
    Serial.println(selectedActivity);

    Serial.println("Client disconnected");
    Serial.println();
  }
}


void showTrace(WiFiClient& client) {
  client.println("<h2>Trajectory</h2>");
  client.println("<p>");
  for(int i = 0; i < next_order; i++){
    client.println("The order number");
    client.println(i + 1);
    client.println(" excuted was ");
    client.println(orders[i]);
    client.println(" and it moved ");
    
    if(orders[i] == 'F'|| orders[i] == 'B'){
      client.println(distances[i] * (twoPi * wheelRadius) / ( stepperResolution * stepToMicrostep));
      client.println(" cm");
    }
    else{
      client.println(distances[i] * (wheelRadius * 720) / (stepperResolution * stepToMicrostep * wheelsAxisDis));
      client.println(" degrees");
    }
    client.println("<br/>");
  }
  client.println("</p>");
}