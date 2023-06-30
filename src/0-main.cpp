#include "Config.h"

// Define the EEPROM address to store the maximum voltage
const float max_voltage_eeprom = 0; 

float checkVoltage(const uint8_t pin) {
    const float R1 = 30000.0;
    const float R2 = 7500.0;
    
    float adc_value = analogRead(pin);
    float adc_voltage = (adc_value * 3.3) / 4095.0;
    float in_voltage = adc_voltage / (R2 / (R1 + R2));
    
    return in_voltage;
}

float checkMaxVoltRead(float volt) {
    float max_volt;
    
    EEPROM.get(max_voltage_eeprom, max_volt);
    
    if (isnan(max_volt) || volt > max_volt) {
        max_volt = volt;
        EEPROM.put(max_voltage_eeprom, max_volt);
        EEPROM.commit();
    }
    
    return max_volt;
}

float checkIrradiance(const uint8_t pin) {
    float voltage = checkVoltage(pin);
    float max_volt = checkMaxVoltRead(voltage);
    
    return 1000.0 * (voltage / max_volt);
}


void blink() {
    digitalWrite(2, HIGH);
}