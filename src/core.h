#ifndef ESP8266_FAIRY_LIGHTS_CORE_H
#define ESP8266_FAIRY_LIGHTS_CORE_H

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

#ifndef BLINK_DELAY_MS
#define BLINK_DELAY_MS 2000
#endif

#ifndef FAST_BLINK_DELAY_MS
#define FAST_BLINK_DELAY_MS 2000
#endif

#ifndef UPDATE_DELAY
#define UPDATE_DELAY 5
#endif

#include <Arduino.h>
#include "core_types.h"
#include "web_server_types.h"


class Core {
public:
    Core() = default;

    write_channel_status_t write_channel(uint8_t channel, uint8_t val, bool force = false) {
        if (channel > channels_length) return CHANNEL_OUTSIDE;
        if (channels == nullptr) return NOT_INITED_CHANNELS;
        Channel &ch = channels[channel];
        if (ch.is_not_inited) return CHANNEL_NOT_INITED;
        ch.bright = val;
        if (force) write_bright_CRT(ch);
        else ch.is_need_update = true;
        return CHANNEL_WRITE_OK;
    }

    init_channel_status_t init_channels(Channel *channels_ptr, uint8_t len) {
        if (len < 1) return init_channel_status_t::INIT_CHANNEL_ERROR_LENGTH;
        channels = channels_ptr;
        channels_length = len;
        for (int i = 0; i < len; ++i) {
            if (channels[i].pin < 0) continue;
            pinMode(channels[i].pin, OUTPUT);
            channels[i].is_not_inited = false;
            write_bright_CRT(channels[i]);
        }
        return init_channel_status_t::INIT_CHANNEL_OK;
    }

    init_sources_status_t init_led_source(LightSource *sources_ptr, uint8_t len) {
        if (len < 1) return init_sources_status_t::INIT_SOURCES_ERROR_LENGTH;
        sources = sources_ptr;
        return init_sources_status_t::INIT_SOURCES_OK;
    }

    static void write_bright_CRT(Channel &channel) {
        uint8_t val = channel.bright;
        val = ((long)val * val + 255) >> 8;
        analogWrite(channel.pin, val);
        channel.is_need_update = false;
    }

    void blink() {
        if (millis() - last_blink < BLINK_DELAY_MS) return;
        last_blink = millis();
        states.led ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
        states.led = !states.led;
    }
    void fast_blink() {
        if (millis() - last_fast_blink < FAST_BLINK_DELAY_MS) return;
        last_fast_blink = millis();
        states.led ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
        states.led = !states.led;
    }

    void update() {
        if (millis() - last_update < UPDATE_DELAY) return;
        last_update = millis();
        for (int i = 0; i < channels_length; ++i) {
            Channel &ch = channels[i];
            if (!ch.is_need_update) continue;
            Serial.println(ch.is_need_update);
            write_bright_CRT(ch);
        }
    }

    void write_channels_bright_to_buffer(uint8_t *buffer) {
        for (int i = 0; i < channels_length; ++i) {buffer[i] = channels[i].bright; }
    }

    [[nodiscard]] uint8_t get_len_channels() const {
        return channels_length;
    }

private:
    LightSource *sources = nullptr;
    Channel *channels = nullptr;
    uint8_t channels_length = 0;
    uint8_t sources_length = 0;
    States states{};
    uint32_t last_blink = 0;
    uint32_t last_fast_blink = 0;
    uint32_t last_update = 0;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
