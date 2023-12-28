#ifndef ESP8266_FAIRY_LIGHTS_CORE_H
#define ESP8266_FAIRY_LIGHTS_CORE_H

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

#ifndef BLINK_DELAY_MS
#define BLINK_DELAY_MS 100
#endif

#ifndef UPDATE_CORE_DELAY
#define UPDATE_CORE_DELAY 5
#endif

#include <Arduino.h>
#include "core_types.h"
#include "color_modes.h"
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
        init_sources_status_t status = init_sources_status_t::INIT_SOURCES_OK;
        sources = sources_ptr;
        sources_length = len;
        for (int i = 0; i < len; ++i) {
            if (change_color_mode_source(sources[i], sources->mode) == CHANGE_OK) continue;
            status = INIT_SOURCES_OK_WITH_NODES;
        }
        return status;
    }

    static change_color_mode_source_status_t change_color_mode_source(LightSource &source, ColorMode new_color_mode) {
        if (source.mode == new_color_mode && source.color_mode_data != nullptr)
            return change_color_mode_source_status_t::CHANGE_PASS;
        delete[] source.color_mode_data;
        switch (new_color_mode) {
            case OFF_MODE:
                source.mode = OFF_MODE;
                break;
            case COLOR_MODE:
                source.mode = COLOR_MODE;
                break;
            case SMOOTH_MODE:
                source.mode = SMOOTH_MODE;
                source.color_mode_data = reinterpret_cast<uint8_t*>(new SmoothModeData{});
                break;
            case FADE_MODE:
                source.mode = FADE_MODE;
                source.color_mode_data = reinterpret_cast<uint8_t*>(new FadeModeData{});
                break;
        }
        return change_color_mode_source_status_t::CHANGE_OK;
    }

    static void show_color_mode_source(LightSource &source) {
        ChannelCount len = source.ch2 == nullptr ? ChannelCount::ONE : source.ch3 == nullptr ? ChannelCount::TWO : ChannelCount::THREE;
        ChannelsBright bright;
        bool need_update = false;
        switch (source.mode) {
            case OFF_MODE:
            case COLOR_MODE:
                switch (len) {
                    case THREE: bright.ch3 = source.ch3->bright;
                    case TWO  : bright.ch2 = source.ch2->bright;
                    case ONE  : bright.ch1 = source.ch1->bright;
                }
                need_update = true;
                break;
            case SMOOTH_MODE:
                need_update = ColorModes::calculate_smooth_color_mode(
                        bright, *reinterpret_cast<SmoothModeData*>(source.color_mode_data)
                );
                break;
            case FADE_MODE:

                need_update = ColorModes::calculate_fade_color_mode(
                        bright, *reinterpret_cast<FadeModeData*>(source.color_mode_data)
                );
                break;
        }
        if (!need_update) return;
        switch (len) {
            case THREE: write_bright_CRT(source.ch3->pin, bright.ch3);
            case TWO  : write_bright_CRT(source.ch2->pin, bright.ch2);
            case ONE  : write_bright_CRT(source.ch1->pin, bright.ch1);;
        }
    }

    static void write_bright_CRT(Channel &channel) { write_bright_CRT(channel.pin, channel.bright); }

    static void write_bright_CRT(uint8_t pin, uint8_t val) {
        val = ((long)val * val + 255) >> 8;
        analogWrite(pin, val);
    }

    void blink() {
        if (millis() - last_blink < BLINK_DELAY_MS) return;
        last_blink = millis();
        states.led ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
        states.led = !states.led;
    }

    void update() {
        if (millis() - last_update < UPDATE_CORE_DELAY) return;
        for (int i = 0; i < sources_length; ++i) show_color_mode_source(sources[i]);
        last_update = millis();
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
    uint32_t last_update = 0;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
