#include <Arduino.h>
#include <String.h>
#include <ESP8266WiFi.h>
#include <DMotor_mod.h>

#include "../include/mainClient.h"
#include "../include/physicalData.h"

// FUNCTION 10: MENÚ PRINCIPAL: ------------------------------------------------------------------------------------------//
void principalMenu(WiFiServer* server, int& next_order, int& nextActivity, int& actividadSel) {

  // Stay within the principal menu until the order is told
  while (actividadSel == 0) {
  
    //------------------------ DO NOT CHANGE --------------------------//
    WiFiClient client = server->available();
    while (!client) {
      // Check if a client has connected
      client = server->available();
    }
    
    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available()) {
      delay(1);
    }
    
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    //---------------------- END OF DO NOT CHANGE ---------------------//
   
    // Match the request
    int pos = request.lastIndexOf('-');

    next_order = 0;

    //-------------------------- ALGORITHMS ---------------------------//

    if (request.indexOf("/A1") != -1) {
      nextActivity = 1;
      }
    if (request.indexOf("/A2") != -1) {
      nextActivity = 2;
    }
    if (request.indexOf("/BACK") != -1) {
      nextActivity = 0;
    }

    //---------------------- END OF ALGORITHMS ------------------------//

   
    //***************************A PARTIR DE AQUÍ VAMOS A TRABAJAR**********************************************************************************************************************************************************************************  
    // Return the response
    client.println("HTTP/1.1 200 OK\
    Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE html>\
    <html>\
      <head>\
        <title> LLUBot </title>");

    //************************************************************** CSS ************************************************************************************************************************************************************//

    client.println("<style>\
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana}\
        .main button {  height: 150px; width: 80%; font-size: 25px; }\
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}\
        .button {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}\
      h1 {font-size: 50px;}\
      a {color: white; text-decoration: none}\
      body {font-size: 30px; font-family: Verdana}\
    </style>");

    //************************************************************* HTLM ************************************************************************************************************************************************************//
    client.println("</head> \
    <body>\
      <div class='main'>\
        <h1> LLUBot </h1>\
        \
        \
        <div class= 'button'>\
        <a href=\"/A1\">\
          <button class= 'buttonA1'>\
            Roomba \
          </button>\
        </a>\
          <br></br>\
        \
        \
          <a href=\"/A2\">\
            <button class= 'buttonA2'> Actividad: Chess  </button>\
          </a>\
        </div>\
        <br></br>\
        \
        \
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <br></br>\
        <p> Welcome to the Nether </p>\
      </div>\
    </body>\
    </html>");
    
    //**************ÓRDENES RECIBIDAS**********************//  

    actividadSel = nextActivity;
    Serial.println("Actividad actual: 0");
    Serial.print("Actividad seleccionada: ");
    Serial.println(actividadSel);

    //************************* FIN DEL SERVIDOR ***************************************************/

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
    client.flush();
  }
}


void mostrarTraza(WiFiClient& client, const int& next_order, char* orders, const int* distances) {
  client.println("<h2>Trajectory</h2>");
  client.println("<p>");
  for(int i = 0; i < next_order; i++){
    client.println("The order number");
    client.println(i+1);
    client.println(" excuted was ");
    client.println(orders[i]);
    client.println(" and it moved ");
    
    if(orders[i] == 'F'|| orders[i] == 'B'){
      //steps_ = cm_ * stepperResolution * stepToMicrostep / (twoPi * wheelRadius); 
      client.println(distances[i] * (twoPi * wheelRadius) / ( stepperResolution * stepToMicrostep));
      client.println(" cm");
    }
    else{
      //steps_ = angle_ * stepperResolution * stepToMicrostep * wheelsAxisDis / (wheelRadius * 720);
      client.println(distances[i] * (wheelRadius * 720) / (stepperResolution * stepToMicrostep * wheelsAxisDis));
      client.println(" degrees");
    }
    client.println("<br/>");
  }
  client.println("</p>");
}