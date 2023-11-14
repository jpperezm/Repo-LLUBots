//***********************************CONFIGURACIÓN*************************************************

void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(voltagePin, INPUT); 
  pinMode(ledPin, OUTPUT);

  sensorServo.attach(servoPin);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");


//************IMPORTANTE*************//
 
  // IMPRIMIR LA DIRECCIÓN IP
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

//***********************************//

  // Steppers speed
  motorR.setSpeed(chosenSpeed);
  motorL.setSpeed(chosenSpeed);   

  // Init servo at 90 degrees
  sensorServo.write(servoAngle); 

  Serial.print("Empiezo en la actividad: ");
  Serial.println(actividadSel);
  actividadSel = 0;
  valueA1 = 0; // initialization in 0 for Roomba in activity 1
  valueA2 = 0; // initialization in 0 for activity 2

}
 
