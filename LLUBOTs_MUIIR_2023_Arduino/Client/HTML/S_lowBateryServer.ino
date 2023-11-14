// FUNCTION 6: LOW BATTERY SERVER: ----------------------------------------------------------------------------------------//
void lowBatteryServer() {

  //------------------------ DO NOT CHANGE --------------------------//
  WiFiClient client = server.available();
  while(!client){
    // Check if a client has connected
    client = server.available();
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  //---------------------- END OF DO NOT CHANGE ---------------------//

  //---------------------------- SERVER -----------------------------//
  client.println("HTTP/1.1 200 OK \
    Content-Type: text/html");
    client.println("");        // IMPORTANT: do not forget this one
    client.println("<!DOCTYPE html> \
    <html lang='es'> \
      <head> \
        <title> LLUBot: Low Battery </title> \
        <style> \
            body { background-color: #7c05a3; \
                   font-family: Verdana; \
                   text-align: center; \
                   font-size: 50px; \
                   color: white } \
        </style> \
      </head> \
      <body> \
        <p> LOW BATTERY! </p> \
        <br/> \
        <p> Please turn the LLUBot off and charge it completely.</p> \
      </body> \
    </html>");
  //--------------------------- END OF SERVER -----------------------//

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
  client.flush();  
}
