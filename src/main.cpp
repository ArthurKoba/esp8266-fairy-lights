#include <Arduino.h>
#include "configs.h"
#include "core.h"
#include "web_server.h"

const char ssid[]  = "";
const char pass[] = "";
Core core;
WebServer server;


void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, pass);
    server.init([] (const uint8_t *d, size_t s) {core.data_handler(d, s);});
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) core.fast_blink();
    else core.blink();
}