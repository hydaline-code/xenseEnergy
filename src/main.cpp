#include "Config.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(512);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);

  IPAddress apIP(192, 168, 1, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  delay(2000);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  socketRun();
  startHTTPServer();
}

void loop() {
  sendData();
  switchPower(LED_BUILTIN, SENSOR_PIN);
}
