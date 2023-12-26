#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "web_server_files/files.h"

enum WebServerFile {INDEX, STYLES, APP_JS, NOT_FOUND_HTML};

const char* HTML_TYPE     PROGMEM = "text/html";
const char* CSS_TYPE      PROGMEM = "text/css";
const char* SCRIPT_TYPE   PROGMEM = "application/javascript";
const char* PLAIN_TYPE    PROGMEM = "text/plain";
const char* OCTET_TYPE    PROGMEM = "application/octet-stream";
const char* PLAIN         PROGMEM = "plain";

typedef AsyncWebServerRequest Req;
typedef void (*DataHandler)(const uint8_t *, size_t);

class WebServer {
public:
    WebServer() = default;

    void init(DataHandler data_handler) {
        handler = data_handler;
        srv.on("/update", HTTP_POST,
               [this] (Req *r) {r->send(200, PLAIN_TYPE, "OK");},
               nullptr,
               [this] (Req *r, const uint8_t *data, size_t len, size_t index, size_t total) {
            update_handler(data, len);
        });
        srv.on("/", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::INDEX);});
        srv.on("/styles.css", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::STYLES);});
        srv.on("/app.js", HTTP_GET, [this] (Req *r) {page_handler(r, WebServerFile::APP_JS);});
        srv.onNotFound( [this] (Req *r) {page_handler(r, WebServerFile::NOT_FOUND_HTML);});
        srv.begin();
    }

protected:
    void update_handler(const uint8_t *data, size_t len) {
//        if (handler == nullptr) return srv.send(500, PLAIN_TYPE, PSTR("Server data handler error"));
//        else if (!srv.hasArg(PLAIN)) return srv.send(500, PLAIN_TYPE, PSTR("Body not received"));
//        else if (srv.arg(PLAIN).length() != 3) return srv.send(500, PLAIN_TYPE, PSTR("Error parse: body len not is equals 3"));
        Serial.print("Start data: ");
        for (size_t i = 0; i < len - 1; ++i) {
            Serial.print(data[i]);
            Serial.print(", ");
        }
        Serial.print(data[len-1]);
        Serial.println(". End.");
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
