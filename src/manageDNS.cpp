#include "Config.h"
#include <DNSServer.h>
#include <EasyDDNS.h>

DNSServer dnsAP;

const char* duckDNSToken = "f976eaf4-3962-47b9-b8f3-7a4c432abfdb";
const char* duckDomain = "xenseenergy.duckdns.org";


void runNetwork() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(SSID, PASSWORD);
    IPAddress apIP(192, 168, 1, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  
    dnsAP.start(53, DOMAIN,apIP);
    EasyDDNS.client(duckDomain, duckDNSToken);
    EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP){
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });
}

void dnsUpdater() {
  dnsAP.processNextRequest();
  /* EasyDDNS.update(10000, true); */
}
