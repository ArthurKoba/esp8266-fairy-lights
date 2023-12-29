#ifndef ESP8266_FAIRY_LIGHTS_CORE_TYPES_H
#define ESP8266_FAIRY_LIGHTS_CORE_TYPES_H

#include <Arduino.h>

typedef enum {
    INIT_CHANNEL_OK = 0,
    INIT_CHANNEL_ERROR_LENGTH = 1
} init_channel_status_t;

typedef enum {
    INIT_SOURCES_OK = 0,
    INIT_SOURCES_ERROR_LENGTH = 1,
    INIT_SOURCES_OK_WITH_NODES = 2
} init_sources_status_t;

typedef enum {
    CHANGE_OK = 0,
    CHANGE_PASS = 1
} change_color_mode_source_status_t;

typedef enum {
    CHANNEL_WRITE_OK = 0,
    CHANNEL_OUTSIDE,
    CHANNEL_NOT_INITED,
    NOT_INITED_CHANNELS //
} write_channel_status_t;

enum ColorMode {
    OFF_MODE = 0,
    COLOR_MODE = 1,
    FADE_MODE,
    SMOOTH_MODE,
};

struct Channel {
    int8_t pin = -1;
    uint8_t bright = 0;
    bool is_not_inited = true;
};

struct LightSource {
    Channel *ch1 = nullptr;
    Channel *ch2 = nullptr;
    Channel *ch3 = nullptr;
    ColorMode mode = OFF_MODE;
    uint8 *color_mode_data = nullptr;
};

const uint16_t crt_gamma_pgm_10_bit[256] PROGMEM  = {
        0, 0 ,0,1,1,1,1,1,1,1,1,1,1,2,2,2,
        2,2,2,2,2,2,2,3,3,3,4,4,4,5,5,6,
        6,7,7,8,8,9,9,10,11,11,12,13,14,14,15,16,
        17,18,19,20,20,21,22,24,25,26,27,28,29,30,32,33,
        34,35,37,38,40,41,43,44,46,47,49,50,52,54,56,57,
        59,6,63,65,67,69,71,73,75,77,79,81,83,86,88,90,
        93,95,97,100,102,105,107,110,113,115,118,121,124,126,129,132,
        135,138,141,144,147,151,154,157,160,164,167,170,174,177,181,184,
        188,191,195,199,203,206,210,214,218,222,226,230,234,238,242,247,
        251,255,260,264,268,273,277,282,287,291,296,301,305,310,315,320,
        325,330,335,340,345,351,356,361,367,372,377,383,388,394,400,405,
        411,417,423,428,434,440,446,452,458,465,471,477,483,490,496,503,
        509,516,522,529,536,542,549,556,563,570,577,584,591,598,605,613,
        620,627,635,642,650,657,665,672,680,688,696,704,712,720,728,736,
        744,752,760,769,777,785,794,802,811,820,828,837,846,855,863,872,
        881,890,900,909,918,927,937,946,955,965,975,984,994,1004,1013,1023
};
#endif //ESP8266_FAIRY_LIGHTS_CORE_TYPES_H
