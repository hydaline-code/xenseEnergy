#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <WiFiClientSecure.h>

#define SSID "xenseEnergy"
#define PASSWORD "12345678"
#define DOMAIN "xenseEnergy.tech"
#define IRRADIANCE_MAX_VALUE 1000.0
#define LED_BUILTIN 2
#define MAX_CLIENTS 8
#define SENSOR_PIN 34
#define MIN_VOLT_ADDR 8
#define MAX_VOLT_ADDR 16



double checkVoltage(const uint8_t pin);
double checkIrradiance(const uint8_t pin);
void blink();
void socketRun();
void sendData();
void startHTTPSServer();
void startHTTPServer();
String readVoltStoreInEEPROM();
bool writeVoltStoreInEEPROM(const String& client_irr_settings);
void switchTest(const String& message);
String activeClientsJson(int clients);
void switchPower(uint8_t powerPin, uint8_t sensorPin);
void runNetwork();
void dnsUpdater();
extern double irradiance;


#endif  // CONFIG_H
