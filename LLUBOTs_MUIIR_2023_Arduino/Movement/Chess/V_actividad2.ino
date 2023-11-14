// ACTIVIDAD 2:
void maths(){
  while (actividadSel==2){

  //*******************************NO TOCAR***************************//
  WiFiClient client = server.available();
  while(!client){
    // Check if a client has connected
    client = server.available();
//    if (!client) {
//      return;
//    }
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

  if (request.indexOf("/MOTOR=KNIGHT") != -1) {
    value = 2;
    valueA2 = 1;
    request.remove(0,pos+1);
    request.remove(2,request.length());
    movA2 = request;
    Serial.println("Movement selected: Knight= " + movA2);
    }
  if (request.indexOf("/MOTOR=KING") != -1) {
    value = 2;
    valueA2 = 2;
    request.remove(0,pos+1);
    movA2 = request;
  }
  if (request.indexOf("/MOTOR=QUEEN") != -1) {
    value = 2;
    valueA2 = 3;
    request.remove(0,pos+1);
    request.remove(3,request.length());
    
    movA2 = request;
  }
  if (request.indexOf("/MOTOR=BISHOP") != -1) {
    value = 2;
    valueA2 = 4;
    request.remove(0,pos+1);
    request.remove(3,request.length());

    movA2 = request;
  } 
  if (request.indexOf("/MOTOR=ROOK") != -1) {
    value = 2;
    valueA2 = 5;
    request.remove(0,pos+1);
    request.remove(2,request.length());

    movA2 = request;
  } 
  if (request.indexOf("/MOTOR=PAWN") != -1) {
    value = 2;
    valueA2 = 6;
    request.remove(0,pos+1);
    request.remove(2,request.length());
    
    movA2 = request;
  } 
  if (request.indexOf("/BACK") != -1) {
    value=0;
    request.remove(0,pos+1);
    movA2 = request;
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
      <title> LLUBot </title>");

  //************************************************************** CSS ************************************************************************************************************************************************************//

  client.println("<style>\
    body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana;}\
      .main button {  height: 50px; width: 50%; font-size: 25px; }\
      .buttonknight {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .buttonking {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .buttonqueen {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .buttonbishop {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .buttonrook {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .buttonpawn {background-color: white; border-radius: 12px; border: none; color: #7c05a3; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px}\
      .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}\
      .buttongo {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}\
      .menu { font-size: 50px; }\
      .app1 button { font-size: 25px; }\
      .app1 back { font-size: 25px; }\
      .app1 menu { font-size: 25px; }\
    h1 {font-size: 50px;}\
    a {color: white; text-decoration: none}\
    body {font-size: 30px;}\
  </style>");

  //************************************************************* HTLM ************************************************************************************************************************************************************//
  
  client.println("  </head>\
  <body>\
    <div class='main'>\
      <a href=\"/BACK\">\
        <button class= 'back'> Main Menu </button>\
      </a>\
      \
      <h1> LLUBot en el III Laboratorio de Verano de Ciencia y Tecnolog&iacutea de Arona</h1>\
      \
      \
      <div class= 'buttonknight'>\
        Mover el caballo: <input class='num' type='text' id='input1'/> \
        <br></br>\
        <a id='a1' href='' color: white>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br>\
      \
      \
      <div class= 'buttonking'>\
        Avanzar el rey: <input class='num' type='text' id='input2'/>\
        <br></br>\
        <a id='a2' href='' color: white>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br>\
      \
      \
      <div class= 'buttonqueen'>\
        Mover la reina: <input class='num' type='text' id='input3'/>\
        <br></br>\
        <a id='a3' href=''>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br>\
      \
      \
      <div class= 'buttonbishop'>\
        Mover el alf&iacutel: <input class='num' type='text' id='input4'/>\
        <br></br>\
        <a id='a4' href=''>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br>\
      \
      \
      <div class= 'buttonrook'>\
        Mover la torre: <input class='num' type='text' id='input5'/>\
        <br></br>\
        <a id='a5' href=''>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br>\
      \
      \
      <div class= 'buttonpawn'>\
        Mover el pe&oacuten: <input class='num' type='text' id='input6'/> \
        <br></br>\
        <a id='a6' href=''>\
          <button class= 'buttongo'> Go! </button>\
        </a>\
      </div>\
      <br></br><br></br>\
      <div>");

      mostrarTraza(client);

    //********************************************************************* JAVASCRIPT *********************************************************************************************************************************************************************************************************//
  
    client.println("    <script>\
      let main = document.querySelector('.main');\
      let app1 = document.querySelector('.app1');\
      let btnBack = document.querySelectorAll('.back');\
      btnBack.forEach( btn => {\
              btn.addEventListener('click', () => {\
                      main.style.display = 'block';\
                      app1.style.display = 'none';\
              });\
      });\
      let input1 = document.getElementById('input1');\
      let input2 = document.getElementById('input2');\
      let input3 = document.getElementById('input3');\
      let input4 = document.getElementById('input4');\
      let input5 = document.getElementById('input5');\
      let input6 = document.getElementById('input6');\
      let string = '';\
      input1.addEventListener('input', () => string = \"/MOTOR=KNIGHT-\" + input1.value );\
      input2.addEventListener('input', () => string = \"/MOTOR=KING-\" + input2.value );\
      input3.addEventListener('input', () => string = \"/MOTOR=QUEEN-\" + input3.value );\
      input4.addEventListener('input', () => string = \"/MOTOR=BISHOP-\" + input4.value );\
      input5.addEventListener('input', () => string = \"/MOTOR=ROOK-\" + input5.value );\
      input6.addEventListener('input', () => string = \"/MOTOR=PAWN-\" + input6.value );\
      let link1 = document.getElementById('a1');\
      let link2 = document.getElementById('a2');\
      let link3 = document.getElementById('a3');\
      let link4 = document.getElementById('a4');\
      let link5 = document.getElementById('a5');\
      let link6 = document.getElementById('a6');\
      link1.addEventListener('click', () => link1.href = string);\
      link2.addEventListener('click', () => link2.href = string);\
      link3.addEventListener('click', () => link3.href = string);\
      link4.addEventListener('click', () => link4.href = string);\
      link5.addEventListener('click', () => link5.href = string);\
      link6.addEventListener('click', () => link6.href = string);\
    </script>\
  </body>\
  </html>");

   
  //**************ÓRDENES RECIBIDAS**********************//  
  actividadSel=value;
  Serial.println("Actividad actual: 2");
  Serial.print("Actividad seleccionada: ");
  Serial.println(actividadSel);

  if(valueA2  == 1) {      // KNIGHT
    Serial.println("Ejecutando movimiento del caballo...");
    knightMove(movA2);  
    valueA2  = 0;
    movA2 = 0;
  } if(valueA2  == 2) {    // KING
    kingMove(movA2);  
    valueA2 =0;
    movA2 = 0;
  } if(valueA2  == 3) {    // QUEEN
    queenMove(movA2);  
    valueA2 =0;
    movA2 = 0;
  } if(valueA2  == 4) {    // BISHOP
    bishopMove(movA2);  
    valueA2 =0;
    movA2 = 0;
  } if(valueA2  == 5) {    // ROOK
    rookMove(movA2);  
    valueA2 =0;
    movA2 = 0;
  } if(valueA2  == 6) {    // PAWN
    pawnMove(movA2);  
    valueA2 =0;
    movA2 = 0;
  } else {
    actividadSel=value;
    }
  
  //************************* FIN DEL SERVIDOR ***************************************************/

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
  client.flush();

 }
  
}
