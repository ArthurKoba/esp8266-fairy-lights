#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "web_server_files/files.h"

enum WebServerFile {INDEX, STYLES, APP_JS, NOT_FOUND_HTML};

const char* HTML_TYPE     PROGMEM = "text/html";
const char* CSS_TYPE      PROGMEM = "text/css";
const char* SCRIPT_TYPE   PROGMEM = "application/javascript";
const char* PLAIN_TYPE    PROGMEM = "text/plain";
const char* OCTET_TYPE    PROGMEM = "application/octet-stream";

typedef AsyncWebServerRequest Req;
typedef DataProcessingStatus (*DataHandler)(const uint8_t *, size_t);

class WebServer {
public:
    WebServer() = default;

    void init(DataHandler data_handler) {
#ifdef USE_CORS
#warning CORS ENABLED!
        DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
        DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
#endif
        handler = data_handler;
        srv.on("/update", HTTP_POST,
               [this] (Req *r) {},
               nullptr,
               [this] (Req *r, const uint8_t *data, size_t len, size_t index, size_t total) {
            req_data_handler(r, data, len);
        });
        srv.on("/", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::INDEX);});
        srv.on("/styles.css", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::STYLES);});
        srv.on("/app.js", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::APP_JS);});
        srv.onNotFound( [this] (Req *r) {page_handler(r, WebServerFile::NOT_FOUND_HTML);});

        srv.begin();
    }

protected:
    void req_data_handler(Req *r, const uint8_t *data, size_t len) {

        DataProcessingStatus status = handler(data, len);
        Serial.print("Status handler: ");
        Serial.println(status);
        switch (status) {
            case SUCCESS: r->send_P(200, PLAIN_TYPE, PSTR("OK")); break;
            case SYSTEM_ERROR: r->send_P(500, PLAIN_TYPE, PSTR("System Firmware error.")); break;
            case CH_NOT_INITED: r->send_P(406, PLAIN_TYPE, PSTR("Channel not inited")); break;
            case CH_OUTSIDE: r->send_P(406, PLAIN_TYPE, PSTR("Channel beyond initialization")); break;
            case UNKNOWN_PACKET: r->send_P(406, PLAIN_TYPE, PSTR("Packet Unknown")); break;
            case PACKET_LENGTH_ERROR: r->send_P(406, PLAIN_TYPE, PSTR("Packet length error")); break;
        }
    }

    static void page_handler(Req *r, WebServerFile file) {
        switch (file) {
            case INDEX: return r->send_P(200, HTML_TYPE, index_html);
            case NOT_FOUND_HTML: return r->send_P(200, HTML_TYPE, not_found_html);
            case STYLES: return r->send_P(200, CSS_TYPE, styles_css);
            case APP_JS: return r->send_P(200, SCRIPT_TYPE, app_js);
        }
    }

    AsyncWebServer srv = AsyncWebServer(80);
    DataHandler handler = nullptr;
};

#endif //ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
