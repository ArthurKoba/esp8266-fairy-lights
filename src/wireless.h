#ifndef ESP8266_FAIRY_LIGHTS_WIRELESS_H
#define ESP8266_FAIRY_LIGHTS_WIRELESS_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

void connect_wifi(void (*blink)()) {
#if defined(ESP8266)
    wl_status_t status = WiFi.status();
#elif defined(ESP32)
    WiFiClass::mode(WIFI_STA);
    wl_status_t status = WiFiClass::status();
#endif
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    wl_status_t new_status;
    do {
        blink();
#if defined(ESP8266)
        new_status = WiFi.status();
#elif defined(ESP32)
        new_status = WiFiClass::status();
#endif
        if (status != new_status) {
            Serial.printf("Status wi-fi %u\n", status);
        }
        status = new_status;
        delay(100);
    } while (status != WL_CONNECTED);
    Serial.print(PSTR("Wi-Fi connected! IP: "));
    Serial.println(WiFi.localIP());
    digitalWrite(LED_PIN, false);
}

#endif //ESP8266_FAIRY_LIGHTS_WIRELESS_H
