#include <Arduino.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>

/* #include <AsyncWebSocket.h> */

#define SSID "xense"
#define PASSWORD "12345678"
#define DOMAIN "xense"
#define IRRADIANCE_MAX_VALUE 1000.0
#define LED_BUILTIN 2
#define MAX_CLIENTS 8
#define SENSOR_PIN 34
#define MIN_VOLT_ADDR 8
#define MAX_VOLT_ADDR 16


double checkVoltage(const u_int8_t pin);
double checkIrradiance(const u_int8_t pin);
void blink();
void socketRun();
void sendData();
void startHTTPSServer();
void startHTTPServer();
String readVoltStoreInEEPROM();
bool writeVoltStoreInEEPROM(const String& client_irr_settings);
void switchTest(const String& message);
String activeClientsJson(int clients);
void switchPower(u_int8_t powerPin, u_int8_t sensorPin);
extern double irradiance;