#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "configs.h"
#include "core.h"

const char ssid[]  = "";
const char pass[] = "";
Core core;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, pass);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED);
    else core.blink();
}