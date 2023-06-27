#include <Arduino.h>
#include <EEPROM.h>

#define SSID "xense"
#define PASSWORD "12345678"
#define DOMAIN "xense"

float checkVoltage(const u_int8_t pin);
float checkIrradiance(const u_int8_t pin);
