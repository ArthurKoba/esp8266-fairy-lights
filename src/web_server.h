#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "web_server_files/files.h"

enum WebServerFile {INDEX, STYLES, APP_JS, NOT_FOUND_HTML};

const char* HTML_TYPE     PROGMEM = "text/html";
const char* CSS_TYPE      PROGMEM = "text/css";
const char* SCRIPT_TYPE   PROGMEM = "application/javascript";
const char* PLAIN_TYPE    PROGMEM = "text/plain";
const char* PLAIN         PROGMEM = "plain";

class WebServer {
public:
    WebServer() = default;

    void init(uint32_t (*data_handler)(const String &)) {
        handler = data_handler;
        srv.onNotFound([this] {fileHandler(WebServerFile::NOT_FOUND_HTML);});
        srv.on("/", HTTP_GET, [this] { fileHandler(WebServerFile::INDEX); });
        srv.on("/styles.css", HTTP_GET, [this] { fileHandler(WebServerFile::STYLES); });
        srv.on("/app.js", HTTP_GET, [this] { fileHandler(WebServerFile::APP_JS); });
        srv.on("/update", HTTP_POST, [this] {update_handler();});
        srv.begin(80);
    }


    void tick() { srv.handleClient(); }

protected:
    void fileHandler(WebServerFile file) {
        switch (file) {
            case INDEX: return srv.send(200, HTML_TYPE, index_html);
            case NOT_FOUND_HTML: return srv.send(200, HTML_TYPE, not_found_html);
            case STYLES: return srv.send(200, CSS_TYPE, styles_css);
            case APP_JS: return srv.send(200, SCRIPT_TYPE, app_js);
        }
    }

    void update_handler() {
        if (handler == nullptr) return srv.send(500, PLAIN_TYPE, PSTR("Server data handler error"));
        else if (!srv.hasArg(PLAIN)) return srv.send(404, PLAIN_TYPE, PSTR("Body not received"));
        else if (srv.arg(PLAIN).length() != 3) return srv.send(404, PLAIN_TYPE, PSTR("Error parse: body len not is equals 3"));
        uint32_t result = handler(srv.arg(PLAIN));
        srv.send(200, PLAIN_TYPE, "OK");
    }

    uint32_t (*handler)(const String &) = nullptr;
    ESP8266WebServer srv;
};

#endif //ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
