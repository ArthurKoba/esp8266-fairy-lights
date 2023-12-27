#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "configs.h"
#include "core.h"
#include "web_server.h"

const char ssid[]  = "";
const char pass[] = "";
WebServer server;
Core core;
Channel channels[5];


void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));

    channels[0].pin = 5;
    channels[1].pin = 4;
    channels[2].pin = 14;
    channels[3].pin = 12;
    channels[4].pin = 13;

    channels[0].bright = 255;
    channels[1].bright = 255;


    core.init_channels(channels, 5);
    pinMode(LED_PIN, OUTPUT);


    WiFi.begin(ssid, pass);
    server.init(&core);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) core.fast_blink();
    else core.blink();
    core.update();
}