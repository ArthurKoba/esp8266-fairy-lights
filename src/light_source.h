#ifndef ESP8266_FAIRY_LIGHTS_LIGHT_SOURCE_H
#define ESP8266_FAIRY_LIGHTS_LIGHT_SOURCE_H

//#include <Arduino.h>

#include "color_modes.h"
#include "light_source_types.h"


class Source {
public:
    explicit Source(String name) {
        _name = std::move(name);
    }

    ~Source() {
        delete ch1; delete ch2; delete ch3;
    }

    add_channel_status_t add_channel(uint8_t pin) {
        if (count_channels >= 3) return add_channel_status_t::INIT_CHANNEL_FAIL_MAX_CHANNELS;
        if (!ch1) ch1 = new Channel{.pin = pin};
        else if (!ch2) ch2 = new Channel{.pin = pin};
        else if (!ch3) ch3 = new Channel{.pin = pin};
        count_channels++;
        pinMode(pin, OUTPUT);
        return add_channel_status_t::INIT_CHANNEL_OK;
    }

    change_color_mode_source_status_t change_color_mode_source(ColorMode new_color_mode) {
        if (mode == new_color_mode || mode == ColorMode::UNKNOWN_MODE)
            return change_color_mode_source_status_t::CHANGE_PASS;
        mode = new_color_mode;
        switch (new_color_mode) {
            case SMOOTH_MODE: smooth_data.last_show = 0; break;
            case FADE_MODE: fade_data.last_show = 0; break;
            case OFF_MODE: case COLOR_MODE: case UNKNOWN_MODE: break;
        }
        show_mode(true);
        return change_color_mode_source_status_t::CHANGE_OK;
    }

    void show_mode(bool force = false) {
        ChannelsBright bright = {0,0,0};
        switch (mode) {
            case OFF_MODE: case UNKNOWN_MODE: break;
            case COLOR_MODE: force = need_update_color; break;
            case SMOOTH_MODE: force = ColorModes::calculate_smooth_color_mode(bright, smooth_data); break;
            case FADE_MODE: force = ColorModes::calculate_fade_color_mode(bright,fade_data); break;
        }
        if (force) update_bright(bright);
    }

    void update_bright(ChannelsBright &bright) {
        if (ch1) write_bright_CRT(ch1->pin, bright.ch1);
        if (ch2) write_bright_CRT(ch2->pin, bright.ch2);
        if (ch3) write_bright_CRT(ch3->pin, bright.ch3);
    }

    String& get_name() {return _name;}
    ColorMode get_mode() {return mode;}

private:
    static void write_bright_CRT(uint8_t pin, uint8_t val) {
//        uint8_t bright = ((long)val * val + 255) >> 8;
        uint16_t bright = pgm_read_word(&(crt_gamma_pgm_10_bit[val]));
        analogWrite(pin, bright);
    }

    String _name;
    Channel *ch1 = nullptr;
    Channel *ch2 = nullptr;
    Channel *ch3 = nullptr;
    uint8_t count_channels = 0;
    ColorMode mode = OFF_MODE;
    bool need_update_color = false;
    FadeModeData fade_data;
    SmoothModeData smooth_data;
};




#endif //ESP8266_FAIRY_LIGHTS_LIGHT_SOURCE_H
