#include "Config.h"

TaskHandle_t Core0Task;

void dnsCall(void* pvParameters) {
  while (true) {
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  EEPROM.begin(512);


  delay(2000);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  runNetwork();

  socketRun();
  startHTTPServer();
  //xTaskCreatePinnedToCore(dnsCall, "dnsCall", 10000, NULL, 1, &Core0Task, 0);
}

void loop() {
  dnsUpdater();
  sendData();
  switchPower(LED_BUILTIN, SENSOR_PIN);
}
