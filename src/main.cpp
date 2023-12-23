#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "configs.h"

const char ssid[]  = "";
const char pass[] = "";

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, pass);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
    digitalWrite(LED_BUILTIN, LOW);

}