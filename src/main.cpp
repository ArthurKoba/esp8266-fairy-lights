#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "configs.h"
#include "core.h"
#include "web_server.h"

const char ssid[]  = "";
const char pass[] = "";
WebServer server;
Core core;


Channel channels[9] = {
        {.pin = 5},
        {.pin = 4},
        {.pin = 0},

        {.pin = 2},
        {.pin = 14},

        {.pin = 12},
        {.pin = 13},
        {.pin = 15},
};

LightSource sources[3] = {
        {&channels[0], &channels[1], &channels[2], SMOOTH_MODE},
        {&channels[3], &channels[4], nullptr, OFF_MODE},
        {&channels[5], &channels[6], &channels[7], FADE_MODE},
};

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));

    core.init_channels(channels, 5);
    core.init_led_source(sources, 3);
    pinMode(LED_PIN, OUTPUT);

    WiFi.begin(ssid, pass);
    wl_status_t status;
    do {
        core.blink();
        status = WiFi.status();
        delay(100);
    } while (status != WL_CONNECTED);
    digitalWrite(LED_PIN, true);

    server.init(&core);
}

void loop() {core.update();}