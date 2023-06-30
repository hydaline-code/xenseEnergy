#include <Arduino.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <AsyncWebSocket.h>

#define SSID "xense"
#define PASSWORD "12345678"
#define DOMAIN "xense"


float checkVoltage(const u_int8_t pin);
float checkIrradiance(const u_int8_t pin);
void blink();
void socketRun();
void sendData();
void webPage();