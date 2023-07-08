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

void switchTest(const String& message) {
  // Parse the received message as JSON
  StaticJsonDocument<1000> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, message);
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


bool writeVoltStoreInEEPROM(const String& client_irr_settings) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, client_irr_settings);
    if (error) {
        Serial.print("JSON parsing error: ");
        Serial.println(error.c_str());
        return false;
    }

    // Extract values from the JSON data and perform actions based on the received message
    if (doc.containsKey("threshold")) {
        JsonArray threshold = doc["threshold"];
        double min = threshold[0];
        min = round(min * 10.0) / 10.0;
        double max = threshold[1];
        max = round(max * 10.0) / 10.0; 

        double storedMin;
        double storedMax;
        EEPROM.get(MIN_VOLT_ADDR, storedMin);
        EEPROM.get(MAX_VOLT_ADDR, storedMax); 
        if (min > max || max > IRRADIANCE_MAX_VALUE || min < 0)
            return false;
        if (storedMin != min)
            EEPROM.put(MIN_VOLT_ADDR, min);
        if (storedMax != max)
            EEPROM.put(MAX_VOLT_ADDR, max);
        EEPROM.commit();
        
        // Print the extracted threshold values
        if(Serial) {   
            double currentMin;
            double currentMax;
            EEPROM.get(MIN_VOLT_ADDR, currentMin);
            EEPROM.get(MAX_VOLT_ADDR, currentMax);
            Serial.print("Min: ");
            Serial.println(currentMin);
            Serial.print("Max: ");
            Serial.println(currentMax);
        }

        return true;
    }
    else 
        return false;
}
