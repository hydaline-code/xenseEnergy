#include "Config.h"

bool power = true;

// Define the EEPROM address to store the maximum voltage
const u_int8_t max_voltage_eeprom = 0; 

double checkVoltage(const uint8_t pin) {
    const double R1 = 30000.0;
    const double R2 = 7500.0;
    
    double adc_value = analogRead(pin);
    double adc_voltage = (adc_value * 3.3) / 4095.0;
    double in_voltage = adc_voltage / (R2 / (R1 + R2));
    
    return in_voltage;
}

double checkMaxVoltRead(double volt) {
    if (isnan(volt))
        return -1;
    
    double max_volt;
    
    EEPROM.get(max_voltage_eeprom, max_volt);
    
    if (isnan(max_volt) || volt > max_volt) {
        max_volt = volt;
        EEPROM.put(max_voltage_eeprom, max_volt);
        EEPROM.commit();
    }
    
    return max_volt;
}

double checkIrradiance(const uint8_t pin) {
    double voltage = checkVoltage(pin);
    double max_volt = checkMaxVoltRead(voltage);
    
    return IRRADIANCE_MAX_VALUE  * (voltage / max_volt);
}


void blink() {
    digitalWrite(2, HIGH);
}


void switchPower(u_int8_t powerPin, u_int8_t sensorPin) {
    double min;
    double max;
    EEPROM.get(MIN_VOLT_ADDR, min);
    EEPROM.get(MAX_VOLT_ADDR, max);

    if (irradiance >= min && irradiance <= max && power) {
        digitalWrite(powerPin, HIGH);
        power = false;
    } 
    else {
        digitalWrite(powerPin, LOW);
        power = true;
    }
}