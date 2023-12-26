#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "configs.h"
#include "core.h"
#include "web_server.h"

const char ssid[]  = "";
const char pass[] = "";
Core core;
WebServer server;

uint32_t mediator(const String &data) { return core.data_handler(data);}

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, pass);
    server.init(mediator);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) core.fast_blink();
    else core.blink();
}