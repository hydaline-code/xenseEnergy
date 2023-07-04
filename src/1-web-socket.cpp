#include "Config.h"
#include <cmath>

String password = "1245";

AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");

unsigned long currentTime = 0;
unsigned long previousTime = 0;
String irradiance_str = "";
bool check = false;

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
  // Handle WebSocket messages received from the browser
  // You can process the data here and send responses back if needed
  // Example: echoing the received message back to the client
  String message = "";
  for (size_t i = 0; i < len; i++) {
    message += (char)data[i];
  }

  // Parse the received message as JSON
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, message);
  if (error) {
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract values from the JSON data and perform actions based on the received message
  if (jsonDoc.containsKey("action")) {
    String action = jsonDoc["action"].as<String>();

    if (action == "led_on") {
      // Code to turn on an LED or perform any other action
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Received 'led_on' action. Turning on the LED...");
      // ...
    } else if (action == "led_off") {
      // Code to turn off an LED or perform any other action
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Received 'led_off' action. Turning off the LED...");
      // ...
    } else {
      Serial.println("Received unrecognized action: " + action);
    }
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
    // Setup WebSocket event handler
    webSocket.onEvent(onWebSocketEvent);
    // Route for WebSocket connection
    server.addHandler(&webSocket);
    // Pin 2 Output mode for testing
    pinMode(LED_BUILTIN, OUTPUT);
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
    double irradiance = checkIrradiance(SENSOR_PIN);
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


void webPage() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Requesting index page...");
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  server.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/mobile.css", "text/css");
  });

  server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/app.js", "text/javascript");
  });

  server.begin();
}

