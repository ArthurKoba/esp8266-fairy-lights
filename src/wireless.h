#ifndef ESP8266_FAIRY_LIGHTS_WIRELESS_H
#define ESP8266_FAIRY_LIGHTS_WIRELESS_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include "configs.h"


void connect_wifi(void (*blink)()) {
#if defined(ESP32)
    WiFiClass::mode(WIFI_STA);
#endif
    wl_status_t status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    wl_status_t new_status;
    do {
#if defined(ESP8266)
        new_status = WiFi.status();
#elif defined(ESP32)
        new_status = WiFiClass::status();
#endif
        if (status != new_status) {
            Serial.printf("Status wi-fi %u\n", status);
        }
        blink();
        status = new_status;
        delay(100);
    } while (status != WL_CONNECTED);
    digitalWrite(LED_PIN, false);
}

#endif //ESP8266_FAIRY_LIGHTS_WIRELESS_H
