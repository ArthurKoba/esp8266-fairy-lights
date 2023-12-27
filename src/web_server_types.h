#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_TYPES_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_TYPES_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

typedef enum {
    SUCCESS = 0,
    UNKNOWN_PACKET,
    PACKET_LENGTH_ERROR
} data_processing_status_t;

typedef enum {INDEX, STYLES, APP_JS, NOT_FOUND_HTML} web_server_file_t;

const char* HTML_TYPE     PROGMEM = "text/html";
const char* CSS_TYPE      PROGMEM = "text/css";
const char* SCRIPT_TYPE   PROGMEM = "application/javascript";
const char* PLAIN_TYPE    PROGMEM = "text/plain";
const char* OCTET_TYPE    PROGMEM = "application/octet-stream";

typedef AsyncWebServerRequest Req;

struct __attribute__((__packed__)) UpdateChannelPacket {
    uint8_t channel;
    uint8_t bright;
};

#endif //ESP8266_FAIRY_LIGHTS_WEB_SERVER_TYPES_H
