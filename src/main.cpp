
#include "Config.h"

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWORD);

   IPAddress apIP(192, 168, 1, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  /* if (!MDNS.begin("xense")) {
    Serial.println("[ERROR] MDNS responder did not set up");
    while (1) {
      delay(1000);
    }
  } else {
    Serial.println("[INFO] MDNS setup is successful!");
  } */

  
/*   MDNS.begin("xense"); */

  delay(2000);
 

/*   MDNS.addService("http", "tcp", 80); */

  Serial.println("AP mode enabled");
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  webPage();

  socketRun();

}

void loop()
{
  sendData();
  switchPower(LED_BUILTIN, SENSOR_PIN);
  /* String t = readVoltStoreInEEPROM(); */
}
