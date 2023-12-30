#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "configs.h"
#include "core.h"
#include "web_server.h"
#include "light_source.h"

const char ssid[]  = "";
const char pass[] = "";
WebServer server;
Core core;

Source source_one = Source("Fairy Light One");
Source source_two = Source("Fairy Light Two");
Source source_three = Source("Led Strip");

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));
    pinMode(LED_PIN, OUTPUT);
    analogWriteResolution(10);

    source_one.add_channel(4);
    source_one.add_channel(0);

    source_two.add_channel(2);
    source_two.add_channel(14);
    source_two.change_color_mode_source(ColorMode::SMOOTH_MODE);

    source_three.add_channel(12);
    source_three.add_channel(13);
    source_three.add_channel(15);

    Serial.println(PSTR("Channels inited."));
    core.add_light_source(source_one);
    core.add_light_source(source_two);
    core.add_light_source(source_three);
    Serial.println(PSTR("Sources inited. Connect Wi-Fi..."));
    WiFi.begin(ssid, pass);
    wl_status_t status;
    do {
        core.blink();
        status = WiFi.status();
        delay(100);
    } while (status != WL_CONNECTED);

    Serial.print(PSTR("Wi-Fi connected! IP: "));
    Serial.println(WiFi.localIP());
    digitalWrite(LED_PIN, false);

    server.init(&core);
    Serial.println(PSTR("Server successful inited! Start modes."));
}

void loop() {
    core.show_modes();
}