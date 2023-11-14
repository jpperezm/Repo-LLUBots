/* Created by: Luisana Lara Briceño.
 * Graduate in Industrial Electronic Engineering and Automation
 * Contact: luisanalarab@gmail.com
 * LinkedIn: https://www.linkedin.com/in/luisanalarab/
 */

// Libraries are included:
#include <ESP8266WiFi.h>
#include <DMotor_mod.h>
#include <Servo_ESP8266.h>
#include <String.h>

#include "src/Client/HTML/HTML_STRINGS.cpp"
#include "src/Client/HTML/S_lowBatteryServer.cpp"
#include "src/Movement/Chess/U_B_piecesMovement.cpp"
#include "src/Movement/Chess/V_actividad2.cpp"
#include "src/Movement/Roomba/C_roombaTime.cpp"
#include "src/Movement/Roomba/U_actividad1.cpp"






// Connection's data:
#define ledPin D0 //D0 for Wemos D1 R2// D2 for Wemos D1 R1  // for the low battery warning
#define voltagePin D4 //for Wemos D1 R1 and R2 // for the battery voltage detection
#define servoPin D7 //D9 for Wemos D1 R1 // for the use of a servo motor
#define sensorPin A0  // for the use of a analog sensor

// The motors are created with the use of the libraries:
AF_Stepper motorR(256, 1);    // Right motor  (stepper)
AF_Stepper motorL(256, 2);    // Left  motor  (stepper)
Servo_ESP8266 sensorServo;    // Sensor motor (servo)


int n_orders=100;//Maximum number of orders that the LLUBot may execute 
char orders[100];//Orders executed
int distances[100];//Distances moved in each order
int next_order=0;//the next order and distance to fill

// Physical data of the LLUBot:
float wheelRadius = 3.2;      // in cm, check this distance with the physical LLUBot
float wheelsAxisDis = 16.0;   // in cm, check this distance with the physical LLUBot
int stepperResolution = 256;  // 8 bits
int stepToMicrostep = 8;      // 8 microsteps are 1 step within the stepper structure 
float twoPi = 2 * 3.1416;     // geometry use of 2 * pi in the perimeter of a circle
int chosenSpeed = 100;        // in percentage
float chosenLenght = 19.5;    // for activity 3
float chosenAngle = 90;       // for activity 3
float servoAngle = 90.0;      // initial position

// We initialize the battery voltage reading:
int sensorVal = 1;            // for initial state of charged

// Initialize the LLUBot's movement variables according to selected activity:
int valueA1 = 0;              // initialization value for Roomba in activity 1
int valueA2 = 0;              // initialization value for activity 2
float calculatedSteps = 0;    // steps for activity 3
char chosenDirection = 0;     // direction for activities 2 and 3

// Interpolation for the IR sensor
const int tableEntries = 10;  // number of entries for the interpolation table
const int mVinterval  = 100;  // value of the intervals in milivolts
static float sensorDistance[tableEntries] = {80.0,70.0,60.0,50.0,40.0,30.0,25.0,20.0,15.0,10.0};

// Enter your Wi-Fi username and password:
const char* ssid = "WiFi4LLUBots";         // depending on your Wi-Fi connection
const char* password = "LLUBotitos"; // it is not recommended to use this password
//const char* ssid = "MiFibra-6376";         // depending on your Wi-Fi connection
//const char* password = "q56swJWi"; // it is not recommended to use this password

// Initialize the web server on port 80:
WiFiServer server(80);

// We initialize the activity menu:
int actividadSel = 0;         // for the displayed server selection
int value = 0;                // resource for the correct functioning of the previous
int movA1 = 0;                // resource for activity 1
String movA2 = "0";                // resource for activity 2


void mostrarTraza(WiFiClient& client){
  client.println("<h2>Trajectory</h2>");
  client.println("<p>");
  for(int i=0;i<next_order;i++){
    client.println("The order number");
    client.println(i+1);
    client.println(" excuted was ");
    client.println(orders[i]);
    client.println(" and it moved ");
    
    if(orders[i]=='F'||orders[i]=='B'){
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
