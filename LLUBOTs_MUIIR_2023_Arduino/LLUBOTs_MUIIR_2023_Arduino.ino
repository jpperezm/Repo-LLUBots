/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 */

#include <ESP8266WiFi.h>
#include <DMotor_mod.h>
#include <Servo_ESP8266.h>

#include "include/physicalData.h"
#include "include/lowBatteryClient.h"
#include "include/mainClient.h"
#include "include/roombaClient.h"
#include "include/distanceSensing.h"
#include "include/movements.h"
#include "include/roomba.h"

// Connection's data:
#define ledPin D0 //D0 for Wemos D1 R2// D2 for Wemos D1 R1  // for the low battery warning
#define voltagePin D4 //for Wemos D1 R1 and R2 // for the battery voltage detection
#define servoPin D7 //D9 for Wemos D1 R1 // for the use of a servo motor

Servo_ESP8266 sensorServo;    // Sensor motor (servo)
float servoAngle = 90.0;      // initial position of the servo motor

int chosenSpeed = 100;        // in percentage

AF_Stepper motorR(256, 1);    // Right motor  (stepper)
AF_Stepper motorL(256, 2);    // Left  motor  (stepper)

int sensorVal = 1;            // for initial state of charged battery

int n_orders = 100;//Maximum number of oreders that the LLUBot may execute 
char orders[100];//Orders executed
int distances[100];//Distances moved in each order
int next_order = 0;//the next order and distance to fill

int actividadSel = 0;
int nextActivity = 0;

// Enter your Wi-Fi username and password:
const char* ssid = "WiFi4LLUBots";         // depending on your Wi-Fi connection
const char* password = "LLUBotitos"; // it is not recommended to use this password

// Initialize the web server on port 80:
WiFiServer server(80);


//***********************************CONFIGURACIÓN*************************************************

void setup() {
  Serial.begin(115200);
 
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
}


void loop() {
  // First read the voltage value for the correct functioning of the LLUBot.
  digitalWrite(ledPin, LOW);
  sensorVal = 1;//digitalRead(voltagePin);

  // Si NO tenemos suficiente batería:
  if (sensorVal == 0){
    // Turn on the warning LED and show the Low Battery Server:
    digitalWrite(ledPin, HIGH);
    lowBatteryClient(&server, sensorVal);
  } else {
    delay(100);
    switch (actividadSel){
      case 1:
        Serial.println("Hey, entré en la actividad 1");
        juegoRoomba(&server, &sensorServo, motorR, motorL, next_order, orders, distances, sensorVal, nextActivity, actividadSel);
        Serial.println("Hey, salí de la actividad 1");
        break;
      default:
        Serial.println("Hey, estoy en el menú principal");
        principalMenu(&server, next_order, nextActivity, actividadSel);
        Serial.println("Hey, me voy del menú principal");
        break;
    }
  }
}  // FINITE INCANTATEM
