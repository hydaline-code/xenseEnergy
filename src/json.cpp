#include "Config.h"

int8_t checkPinState(u_int8_t pin) {
    return digitalRead(pin) == HIGH? 1 : 0;
}

String readVoltStoreInEEPROM() {
    double min, max;

    EEPROM.get(MIN_VOLT_ADDR, min);
    EEPROM.get(MAX_VOLT_ADDR, max);

    if(isnan(min)) {
        min = 0;
        EEPROM.put(MIN_VOLT_ADDR, min);
        EEPROM.commit();
    }
    if(isnan(max)) {
        max = IRRADIANCE_MAX_VALUE;
        EEPROM.put(MAX_VOLT_ADDR, max);
        EEPROM.commit();
    }

    StaticJsonDocument<100> jsonDoc;
    JsonArray irrArray = jsonDoc.createNestedArray("voltageInMemory");
    irrArray.add(min);
    irrArray.add(max);
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);

    return jsonString;
}

String activeClientsJson(int clients) {
    /* int numConnectedClients = WiFi.softAPgetStationNum(); */
    StaticJsonDocument<100> jsonDoc;
    JsonArray irrArray = jsonDoc.createNestedArray("clients");
    irrArray.add(clients);
    irrArray.add(MAX_CLIENTS);
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    return jsonString;
}

void writeVoltStoreInEEPROM(String volt) {
    /* double irradiance = checkIrradiance(SENSOR_PIN);
    irradiance = round(irradiance * 10.0) / 10.0; */

     StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, volt);
  if (error) {
    Serial.print("JSON parsing error: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract values from the JSON data and perform actions based on the received message
  if (jsonDoc.containsKey("action")) {
    String action = jsonDoc["action"].as<String>();

    if (action == "led_on") {
      // Code to turn on an LED or perform any other action
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Received 'led_on' action. Turning on the LED...");
      // ...
    } else if (action == "led_off") {
      // Code to turn off an LED or perform any other action
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Received 'led_off' action. Turning off the LED...");
      // ...
    } else {
      Serial.println("Received unrecognized action: " + action);
    }
  }
}