

// ACTIVIDAD 1:
void juegoRoomba(){
while (actividadSel==1){
  int nRunRoomba;
  sensorVal=1;//digitalRead(voltagePin);

  if (sensorVal == 0){
    lowBatteryServer();
  }
  else {
    //***************************************NO TOCAR***************************//
    WiFiClient client = server.available();
    while(!client){
      // Check if a client has connected
      client = server.available();
//      if (!client) {
//        return;
//      }
    }
   
    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()){
      delay(1);
    }
   
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
   
    // Match the request
    int pos = request.lastIndexOf('-');
    
    //******************ALGORITMOS*******************//
    if (request.indexOf("/ROOMBA") != -1) {
      valueA1 = 1;
      request.remove(0,pos+1);
      movA1 = request.toInt();
      value = 1;
      }
    if (request.indexOf("/BACK") != -1) {
      value = 0;
    }
    

    //**************FIN ALGORITMOS****************//
  
    //***************************A PARTIR DE AQUÍ VAMOS A TRABAJAR**********************************************************************************************************************************************************************************  
    // Return the response
    client.println("HTTP/1.1 200 OK\
    Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE html>\
    <html>\
      <head>\
        <meta charset=\"UTF-8\">\
        <title> LLUBot: Roomba </title>");

    //************************************************************** CSS ************************************************************************************************************************************************************//

    client.println("<style>\
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana;}\
        .main button {  height: 50px; width: 50%; font-size: 25px; }\
        .buttonRun {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}\
        .buttonGo {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}\
        .menu { font-size: 25px; }\
        .app1 button { font-size: 25px; }\
      h1 {font-size: 50px;}\
      a {color: white; text-decoration: none}\
      body {font-size: 30px;}\
    </style>");

    //************************************************************* HTLM ************************************************************************************************************************************************************//

    client.println("  </head>\
    <body>\
      <div class='main'>\
        <a href=\"/BACK\">\
          <button class= 'menu'> Main Menu </button>\
        </a>\
        <br></br>\
        <h1> LLUBot Roomba </h1>\
        \
        \
        <div class= 'buttonRun'>\
          Run <input class='num' type='text' id='input1'/> veces\
          <br></br>\
          <a id='a1' href='' color: white>\
            <button class= 'buttonGo'> Go! </button>\
          </a>\
        </div>\
        <br></br>\
        <div>\
          ");
        mostrarTraza(client);
        client.println("\
        <br></br>\
        <p> Pulsar el siguiente bot&oacuten para ver la explicaci&oacuten del Roomba.</p>\
        <button id='btn1'> Roomba: Explicaci&oacuten </button><br><br>\
        \
        \
        \
        \
        <img src='https://luisanalarab.files.wordpress.com/2020/07/cuadrc3adcula-mal-hecha.png?w=1024' width='700' height='700'>\
              </div>\
      \
      \
      <div class='app1'>\
        <h1> Roomba </h1>\
          <h2> Thoughts:  </h2>\
            <p> Mobile robotics. </p>\
            <img src='https://www.pequeocio.com/wp-content/uploads/2019/02/perimetro-circunferencia.jpg'>\
            <p> . </p>\
            <p> ? </p>\
          <h2> Code: </h2> \
            <p> . </p>\
            <img src='https://media.monolithicpower.com/wysiwyg/1_11.png'>\
          <button class='back'> Salir de la explicaci&oacuten. </button>\
      </div>");
      
      //********************************************************************* JAVASCRIPT *********************************************************************************************************************************************************************************************************//
    
      client.println("    <script>\
        let main = document.querySelector('.main');\
        let app1 = document.querySelector('.app1');\
        let btnBack = document.querySelectorAll('.back');\
        btn1.addEventListener('click', () => {\
                main.style.display = 'none';\
                app1.style.display = 'block';\
        });\
        btnBack.forEach( btn => {\
                btn.addEventListener('click', () => {\
                        main.style.display = 'block';\
                        app1.style.display = 'none';\
                });\
        });\
        let input1 = document.getElementById('input1');\
        let string = '';\
        input1.addEventListener('input', () => string = \"/ROOMBA-\" + input1.value );\
        let link1 = document.getElementById('a1');\
        link1.addEventListener('click', () => link1.href = string);\
      </script>\
      ");
    
    client.println(" </body>\
    </html>");
 
    
    //**************ÓRDENES RECIBIDAS**********************//  
    actividadSel = value;
    if(valueA1 == 1){
      nRunRoomba = movA1;
      Serial.println(nRunRoomba);
      roombaTime(nRunRoomba);
      valueA1 = 0;
    }

    Serial.println("Actividad actual: 1");
    Serial.print("Actividad seleccionada: ");
    Serial.println(actividadSel);
    
    //************************* FIN DEL SERVIDOR ***************************************************/

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
    client.flush();

    
  }
 }
}
