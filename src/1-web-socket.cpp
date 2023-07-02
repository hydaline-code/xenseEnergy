#include "Config.h"
#define LED_BUILTIN 2
#define MAX_CLIENTS 8

String password = "1245";

AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");

unsigned long currentTime = 0;
unsigned long previousTime = 0;
bool check = false;
float irradiance = 0;
char buffer[10];


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

  // Send the received message back to the client
  // webSocket.textAll(message);
}


void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    // New client connected
    addClient(client);
    uint32_t clientId = client->id();
    Serial.printf("WebSocket client #%u connected\n", client->id());
    webSocket.text(clientId, password);
  } else if (type == WS_EVT_DISCONNECT) {
    // Client disconnected
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    removeClient(client);
  } else if (type == WS_EVT_DATA) {
    // Received WebSocket data
    handleWebSocketMessage(arg, data, len);
      for (int i = 0; i < numConnectedClients; i++) {
        if (connectedClients[i] == client) {
        removeClient(connectedClients[i]);
        break;
      }
    }
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

void sendData() {
  if (numConnectedClients < 1)
    return;
  unsigned long interval = 500;
  currentTime = millis();

  if (currentTime - previousTime > interval && check == false) {
  irradiance = checkIrradiance(36);
  dtostrf(irradiance, 6, 1, buffer);
  String irradiance_str = String(buffer);
  webSocket.textAll(irradiance_str);
  previousTime = currentTime;
  check = true;
  }
  else if (currentTime - previousTime > interval && check == true) {
    check = false;
    previousTime = currentTime;
  }
  else return;
}

/* String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  if (filename.endsWith(".css")) return "text/css";
  if (filename.endsWith(".js")) return "text/javascript";
  // Add more MIME types as needed
  return "application/octet-stream";
} */

/* void webPage() {
  // Enable Gzip compression for the entire server

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
} */

void webPage() {
// Serve the compressed index.html file
server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
  Serial.println("Requesting index page...");
  
  // Get a reference to the response object
  AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/index.html.gz", String(), false);

  // Set the appropriate response headers for compressed content
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("Content-Type", "text/html");
  
  // Send the response
  request->send(response);
});


// Serve the compressed mobile.css file
server.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest* request) {
  // Get a reference to the response object
  AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/mobile.css.gz", "text/css");

  // Set the appropriate response headers for compressed content
  response->addHeader("Content-Encoding", "gzip");
  
  // Send the response
  request->send(response);
});

// Serve the compressed app.js file
server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest* request) {
  // Get a reference to the response object
  AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/app.js.gz", "text/javascript");

  // Set the appropriate response headers for compressed content
  response->addHeader("Content-Encoding", "gzip");
  
  // Send the response
  request->send(response);
});

  server.begin();
}

