#include "Config.h"

const uint8_t max_voltage_eeprom = 4;

float checkVoltage(const u_int8_t pin) {
    const float R1 = 30000.0;
    const float R2 = 7500.0;
    float adc_value = analogRead(pin);
    float adc_voltage  = (adc_value * 3.3) / 4095.0;
    float in_voltage = adc_voltage / (R2/(R1+R2));
    return in_voltage;
}

float checkMaxVoltRead(float volt) {
    if (volt > EEPROM.readFloat(max_voltage_eeprom)) {
        EEPROM.writeFloat(max_voltage_eeprom, volt);
        EEPROM.commit();
        return (volt);
    } 
    else return EEPROM.readFloat(max_voltage_eeprom);
}

float checkIrradiance(const u_int8_t pin) {
    float voltage = checkVoltage(pin);
    float max_volt = checkMaxVoltRead(voltage);
    return (1000.0 * (voltage/max_volt));
}