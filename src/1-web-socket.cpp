#include "Config.h"
#define LED_BUILTIN 2

AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");

unsigned long currentTime = 0;
unsigned long previousTime = 0;
bool check = false;
float irradiance = 0;
char buffer[10];

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
    uint32_t clientId = client->id();
    Serial.printf("WebSocket client #%u connected\n", client->id());
    webSocket.text(clientId, "1245");
  } else if (type == WS_EVT_DISCONNECT) {
    // Client disconnected
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
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

void sendData() {
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

void webPage() {
  // Serve static files
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Requesting index page...");
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  // Serve the CSS file
  server.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/mobile.css", "text/css");
  });

  // Serve the JS file
  server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/app.js", "text/javascript");
  });

  // Start the server
  server.begin();
 /*  MDNS.end(); */
}