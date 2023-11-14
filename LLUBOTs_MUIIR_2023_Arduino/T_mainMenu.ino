// FUNCTION 10: MENÚ PRINCIPAL: ------------------------------------------------------------------------------------------//
void principalMenu(){

  // Stay within the principal menu until the order is told
  while (actividadSel == 0) {
  
    //------------------------ DO NOT CHANGE --------------------------//
    WiFiClient client = server.available();
    while (!client) {
      // Check if a client has connected
      client = server.available();
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

    next_order=0;

    //-------------------------- ALGORITHMS ---------------------------//

    if (request.indexOf("/A1") != -1) {
      value = 1;
      }
    if (request.indexOf("/A2") != -1) {
      value = 2;
    }
    if (request.indexOf("/BACK") != -1) {
      value = 0;
    }

    //---------------------- END OF ALGORITHMS ------------------------//

    //************************************************************** CSS ************************************************************************************************************************************************************//

    client.println(CSS);

    //************************************************************* HTLM ************************************************************************************************************************************************************//
    client.println(MAIN_MENU_HTML);
    
    //**************ÓRDENES RECIBIDAS**********************//  

    actividadSel=value;
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
