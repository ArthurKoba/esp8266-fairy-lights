#include <Arduino.h>

#include "configs.h"
#include "wireless.h"
#include "core.h"
#include "web_server.h"
#include "light_source.h"

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
    source_one.add_channel(SOURCE1_CH1_PIN);
    source_one.add_channel(SOURCE1_CH2_PIN);
    source_one.add_channel(SOURCE1_CH3_PIN);
    source_two.add_channel(SOURCE2_CH1_PIN);
    source_two.add_channel(SOURCE2_CH2_PIN);
    source_two.change_color_mode_source(ColorMode::FADE_MODE);
    source_three.add_channel(SOURCE3_CH1_PIN);
    source_three.add_channel(SOURCE3_CH2_PIN);
    source_three.add_channel(SOURCE3_CH3_PIN);
    Serial.println(PSTR("Channels inited."));

    core.add_light_source(source_one);
    core.add_light_source(source_two);
    core.add_light_source(source_three);
    Serial.println(PSTR("Sources inited. Connect Wi-Fi..."));

    connect_wifi([] () {core.blink();});
    server.init(&core);
    Serial.println(PSTR("Server successful inited! Start modes."));
}

void loop() {
    while(true) {
        core.show_modes();
    };
}