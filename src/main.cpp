#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "configs.h"
#include "core.h"
#include "web_server.h"

const char ssid[]  = "";
const char pass[] = "";
WebServer server;
Core core;

Channel channels[5] = {
        {.pin = 5},
        {.pin = 4},
        {.pin = 14},
        {.pin = 12},
        {.pin = 13},
};

LightSource sources[2] = {
        {&channels[0], &channels[1], nullptr},
        {&channels[2], &channels[3], &channels[4]}
};

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));

    core.init_channels(channels, 5);
    core.init_led_source(sources, 2);
    pinMode(LED_PIN, OUTPUT);


    WiFi.begin(ssid, pass);
    server.init(&core);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) core.fast_blink();
    else core.blink();
    core.update();
}