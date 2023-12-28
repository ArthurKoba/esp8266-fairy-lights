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

#endif //ESP8266_FAIRY_LIGHTS_CORE_TYPES_H
