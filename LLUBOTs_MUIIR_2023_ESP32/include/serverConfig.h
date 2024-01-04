// Enter your Wi-Fi username and password:
extern const char* ssid;
extern const char* password; 

// Initialize the web server on port 80:
extern WiFiServer server;

void connectWifi();
void startServer();
void waitForClient(WiFiClient &client);
String readClientRequest(WiFiClient &client);
