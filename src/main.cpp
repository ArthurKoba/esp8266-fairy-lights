#include <Arduino.h>

#include "configs.h"
#include "wireless.h"
#include "core.h"
#include "web_server.h"
#include "light_source.h"

WebServer server;
Core core = Core(LED_PIN);

Source light_one = Source("Fairy Light One");
Source light_two = Source("Fairy Light Two");
Source led_strip = Source("Led Strip");

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println(PSTR("\nStart system."));

    light_one.add_channel(SOURCE1_CH1_PIN, "Red", 255);
    light_one.add_channel(SOURCE1_CH2_PIN, "Blue and Green", 255);
    light_one.add_channel(SOURCE1_CH3_PIN, "Yellow", 255);
    light_one.set_mode(ColorMode::SMOOTH_MODE);
    light_two.add_channel(SOURCE2_CH1_PIN, "Red and Yellow", 255);
    light_two.add_channel(SOURCE2_CH2_PIN, "Blue and Green", 255);
    light_two.set_mode(ColorMode::SMOOTH_MODE);
    led_strip.add_channel(SOURCE3_CH1_PIN, "Red", 200);
    led_strip.add_channel(SOURCE3_CH2_PIN, "Green");
    led_strip.add_channel(SOURCE3_CH3_PIN, "Blue", 200);
    led_strip.set_mode(ColorMode::COLOR_MODE);
    Serial.println(PSTR("Channels inited."));

    core.add_light_source(light_one);
    core.add_light_source(light_two);
    core.add_light_source(led_strip);
    Serial.println(PSTR("Sources inited. Connect Wi-Fi..."));

    connect_wifi([] () {core.blink();});
    server.init(&core);
    Serial.println(PSTR("Server successful inited! Start modes."));
}

void loop() {
    core.show_modes();
}