#ifndef ESP8266_FAIRY_LIGHTS_COLOR_MODES_H
#define ESP8266_FAIRY_LIGHTS_COLOR_MODES_H

#include <Arduino.h>

struct FadeModeData {
    int8_t index = 0;
    uint8_t delay_ms = 10;
    uint32_t last_show = 0;
};

struct SmoothModeData {
    uint8_t index = 0;
    uint8_t delay_ms = 20;
    uint32_t last_show = 0;
};

struct ChannelsBright {uint8_t ch1 = 0; uint8_t ch2 = 0; uint8_t ch3 = 0;};

enum ChannelCount {ONE = 1, TWO = 2, THREE = 3};

class ColorModes {
public:

    static bool calculate_fade_color_mode(ChannelsBright &channels, FadeModeData &data) {
        if (millis() - data.last_show < data.delay_ms) return false;
        uint8 bright = abs(data.index++);
        if (bright == 128) bright = 255;
        else bright <<= 1;
        channels.ch1 = bright;
        channels.ch2 = bright;
        channels.ch3 = bright;
        data.last_show = millis();
        return true;
    }

    static bool calculate_smooth_color_mode(ChannelsBright &channels, SmoothModeData &data) {
        if (millis() - data.last_show < data.delay_ms) return false;
        if (data.index <= 85) {
            channels.ch1 = map(data.index, 0, 85, 0, 255);
            channels.ch2 = 0;
            channels.ch3 = map(data.index, 0, 85, 255, 0);
        } else if (data.index <= 170) {
            channels.ch1 = map(data.index, 86, 170, 255, 0);
            channels.ch2 = map(data.index, 86, 170, 0, 255);
            channels.ch3 = 0;
        } else {
            channels.ch1 = 0;
            channels.ch2 = map(data.index, 171, 255, 255, 0);
            channels.ch3 = map(data.index, 171, 255, 0, 255);
        }
        data.index++;
        data.last_show = millis();
        return true;
    }
};

#endif //ESP8266_FAIRY_LIGHTS_COLOR_MODES_H
