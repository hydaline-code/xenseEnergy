#include "Config.h"
#include <cmath>

String password = "1245";

AsyncWebServer serverHTTP(80);
AsyncWebServer serverHTTPS(443);
AsyncWebSocket webSocket("/ws");
WiFiClientSecure client;

unsigned long currentTime = 0;
unsigned long previousTime = 0;
String irradiance_str = "";
bool check = false;

double irradiance;

// Define an array to store connected clients
AsyncWebSocketClient* connectedClients[MAX_CLIENTS];
int numConnectedClients = 0;

// Function to add a client to the connectedClients array
void addClient(AsyncWebSocketClient* client) {
  if (numConnectedClients < MAX_CLIENTS) {
    connectedClients[numConnectedClients] = client;
    numConnectedClients++;
  } else {
    // Handle maximum number of clients reached
  }
}

// Function to remove a client from the connectedClients array
void removeClient(AsyncWebSocketClient* client) {
  for (int i = 0; i < numConnectedClients; i++) {
    if (connectedClients[i] == client) {
      // Shift remaining clients to fill the gap
      for (int j = i; j < numConnectedClients - 1; j++) {
        connectedClients[j] = connectedClients[j + 1];
      }
      numConnectedClients--;
      break;
    }
  }
}

void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
  String message = "";
  for (size_t i = 0; i < len; i++) {
    message += (char)data[i];
  }
 /*  switchTest(message); */
 if (writeVoltStoreInEEPROM(message)) {
  String irrInEPPROM = readVoltStoreInEEPROM();
  webSocket.textAll(irrInEPPROM);
 }
}

String getPasscode() {
  String jsonString;
  StaticJsonDocument<50> jsonDoc;
  jsonDoc["passCode"] = password;
  serializeJson(jsonDoc, jsonString);
  return jsonString;
}

void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    // New client connected
    addClient(client);
    uint32_t clientId = client->id();
    Serial.printf("WebSocket client #%u connected\n", client->id());
    String pass = getPasscode();
    String voltageInMemory = readVoltStoreInEEPROM();
    String activeClients = activeClientsJson(numConnectedClients);
    webSocket.text(clientId, pass);
    webSocket.text(clientId, voltageInMemory);  
    webSocket.textAll(activeClients);

  } else if (type == WS_EVT_DISCONNECT) {
    // Client disconnected
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    removeClient(client);
    String activeClients = activeClientsJson(numConnectedClients);
    webSocket.textAll(activeClients);
  } else if (type == WS_EVT_DATA) {
    // Received WebSocket data
    handleWebSocketMessage(arg, data, len);
  }
}

 void socketRun() {
    webSocket.onEvent(onWebSocketEvent);
    serverHTTP.addHandler(&webSocket);
    serverHTTPS.addHandler(&webSocket);
  }

/*
  void sendData() {
  if (numConnectedClients < 1)
    return;

  unsigned long interval = 500;
  currentTime = millis();

  if (currentTime - previousTime > interval && check == false) {
       Serial.println("Send Data");
    irradiance = checkIrradiance(36);
    dtostrf(irradiance, 6, 1, buffer);
    irradiance_str = String(buffer);

    // Iterate over the connected clients and send data to each one
    for (int i = 0; i < numConnectedClients; i++) {
      connectedClients[i]->text(irradiance_str);
    }

    previousTime = currentTime;
    check = true;
  }
  else if (currentTime - previousTime > interval && check == true) {
    check = false;
    previousTime = currentTime;
  }
} */


void sendData() {
  if (numConnectedClients < 1)
    return;
  String jsonString;
  unsigned long interval = 500;
  currentTime = millis();
  
  if (currentTime - previousTime > interval && check == false) {
    irradiance = checkIrradiance(SENSOR_PIN);
    irradiance = round(irradiance * 10.0) / 10.0;
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["irr"] = irradiance;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
    webSocket.textAll(jsonString);
    previousTime = currentTime;
    check = true;
    Serial.print("Number of clients: ");
    Serial.println(numConnectedClients);
  }
  else if (currentTime - previousTime > interval && check == true) {
    check = false;
    previousTime = currentTime;
  }
  else return;
}


void handleAppJsRequest(AsyncWebServerRequest *request) {
  if (request->hasHeader("X-Page-Source") && request->header("X-Page-Source") == "html") {
    request->send(SPIFFS, "/app.js", "text/javascript");
  } else {
    request->send(403); // Forbidden
  }
}

void loadCertificate(WiFiClientSecure& client) {
  File certFile = SPIFFS.open("/certificate.pem", "r");
  if (!certFile) {
    Serial.println("Failed to open certificate file");
    return;
  }

  File privateKeyFile = SPIFFS.open("/private_key.pem", "r");
  if (!privateKeyFile) {
    Serial.println("Failed to open private key file");
    return;
  }

  String certData = certFile.readString();
  String privateKeyData = privateKeyFile.readString();

  client.setCACert(certData.c_str());
  client.setPrivateKey(privateKeyData.c_str());
}


void startHTTPServer() {
  serverHTTP.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requesting index page (HTTP)...");
    request->send(SPIFFS, "/index.html", "text/html");
  });

  serverHTTP.on("/app.js", HTTP_GET, handleAppJsRequest);
  serverHTTP.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/mobile.css", "text/css");
  });
  serverHTTP.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });

  // Start the HTTP server
  serverHTTP.begin();
  Serial.println("HTTP server started");
}

void startHTTPSServer() {
  // Load SSL certificate and private key
  client.setCACert("/certificate.pem");
  client.setCertificate("/certificate.pem");
  client.setPrivateKey("/private_key.pem");

  // Configure SSL context for the HTTPS server
  serverHTTPS.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requesting index page (HTTPS)...");
    request->send(SPIFFS, "/index.html", "text/html");
  });

  serverHTTPS.on("/app.js", HTTP_GET, handleAppJsRequest);
  serverHTTPS.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/mobile.css", "text/css");
  });
  serverHTTPS.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });

  // Start the HTTPS server
  serverHTTPS.begin();
  Serial.println("HTTPS server started");
}
