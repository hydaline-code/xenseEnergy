#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include "Config.h"

AsyncWebServer server(80);
const char* domain = DOMAIN;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);

  pinMode(34, INPUT);

   IPAddress apIP(192, 168, 1, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  /* if (!MDNS.begin("xense")) {
    Serial.println("[ERROR] MDNS responder did not set up");
    while (1) {
      delay(1000);
    }
  } else {
    Serial.println("[INFO] MDNS setup is successful!");
  } */

  
  MDNS.begin("xense");

  delay(2000);
 

  MDNS.addService("http", "tcp", 80);

  Serial.println("AP mode enabled");
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Serve static files
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Requesting index page...");
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  // Serve the CSS file
  server.on("/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/mobile.css", "text/css");
  });

   server.on("/login.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/login.css", "text/css");
  });

  // Serve the JS file
  server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/app.js", "text/javascript");
  });

  // Start the server
  server.begin();
 /*  MDNS.end(); */

  Serial.println("Server started");
}

void loop()
{

   // Print results to Serial Monitor to 2 decimal places
 /*  Serial.print("Input Voltage = ");
  Serial.println(checkVoltage(36), 2); */

  Serial.print(checkIrradiance(36));
  Serial.println("W/m");
  
  // Short delay
  delay(500);
}
