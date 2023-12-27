#ifndef ESP8266_FAIRY_LIGHTS_CORE_TYPES_H
#define ESP8266_FAIRY_LIGHTS_CORE_TYPES_H

#include <Arduino.h>

typedef enum {INIT_CHANNEL_OK = 0, INIT_CHANNEL_ERROR_LENGTH = 1} init_channel_status_t;
typedef enum {INIT_SOURCES_OK = 0, INIT_SOURCES_ERROR_LENGTH = 1} init_sources_status_t;
typedef enum {
    CHANNEL_WRITE_OK = 0,
    CHANNEL_OUTSIDE,
    CHANNEL_NOT_INITED,
    NOT_INITED_CHANNELS //
} write_channel_status_t;

enum ColorMode {
    OFF_MODE = 0,
};

struct States {
    bool led : 1;
    bool flag1 : 1;
    bool flag2 : 1;
    bool flag3 : 1;
    bool flag4 : 1;
    bool flag5 : 1;
    bool flag6 : 1;
    bool flag7 : 1;
};

struct Channel {
    int8_t pin = -1;
    uint8_t bright = 0;
    bool is_not_inited = true;
    bool is_need_update = false;
};

struct LightSource {
    Channel *ch1 = nullptr;
    Channel *ch2 = nullptr;
    Channel *ch3 = nullptr;
    uint8_t state = 0;
    ColorMode mode = OFF_MODE;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_TYPES_H
