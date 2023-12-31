#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <ESPAsyncWebServer.h>

#include "web_server_types.h"
#include "web_server_files/files.h"

class WebServer {
public:
    WebServer() = default;

    void init(Core *core) {
#ifdef USE_CORS
        DefaultHeaders::Instance().addHeader(PSTR("Access-Control-Allow-Origin"), PSTR("*"));
        DefaultHeaders::Instance().addHeader(PSTR("Access-Control-Allow-Headers"), PSTR("content-type"));
#warning "CORS ENABLED!"
#endif
        c = core;
        srv.on("/changeBright", HTTP_POST, [] (Req *r) {},nullptr,
        [this] (Req *r, const uint8_t *d, size_t l, size_t i, size_t t) {change_bright(r, d, l);
        });
        srv.on("/changeMode", HTTP_POST, [] (Req *r) {},nullptr,
       [this] (Req *r, const uint8_t *d, size_t l, size_t i, size_t t) {change_mode(r, d, l);
        });
        srv.on("/getSourcesInfo", HTTP_GET, [this] (Req *r) {get_sources_info(r);});

        srv.on("/", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::INDEX);});
        srv.on("/styles.css", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::STYLES);});
        srv.on("/app.js", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::APP_JS);});
        srv.onNotFound( [] (Req *r) {page_handler(r, web_server_file_t::NOT_FOUND_HTML);});
        srv.begin();
    }

protected:
    void change_mode(Req *r, const uint8_t *data, size_t len) {
        if (len != 2) return r->send_P(500, PLAIN_TYPE, PSTR("Length not equals 2 byte"));
        uint8_t source_id = data[0];
        uint8_t mode = data[1];
        change_mode_status_t status = c->change_mode(source_id, mode);
        switch (status) {
            case CHANGE_MODE_OK:
                return r->send_P(200, PLAIN_TYPE, PSTR("OK"));
            case CHANGE_MODE_FAIL_OUTSIDE_SOURCE:
                return r->send_P(500, PLAIN_TYPE, PSTR("Source outsize"));
            case CHANGE_MODE_FAIL_OUTSIDE_MODE:
                return r->send_P(500, PLAIN_TYPE, PSTR("Mode outsize"));
        }
    }

    void get_sources_info(Req *r) {
        auto *buffer = new uint8_t[c->get_source_info_buffer_length()];
        size_t buffer_size = c->get_sources_info(buffer);
        if (!buffer_size) return r->send(500, PLAIN_TYPE, PSTR("NO SOURCES"));;
        r->send_P(200, OCTET_TYPE, buffer, buffer_size);
        delete[] buffer;
    }

    void change_bright(Req *r, const uint8_t *data, size_t len) {
        if (len != 3) return r->send_P(500, PLAIN_TYPE, PSTR("Length not equals 3 byte"));
        change_bright_status_t status = c->change_bright(data[0], data[1], data[2]);
        switch (status) {
            case CHANGE_BRIGHT_OK:
                return r->send_P(200, PLAIN_TYPE, PSTR("OK"));
            case CHANGE_BRIGHT_FAIL_OUTSIDE_SOURCE:
                return r->send_P(500, PLAIN_TYPE, PSTR("Source outsize"));
            case CHANGE_BRIGHT_FAIL_OUTSIDE_CHANNEL:
                return r->send_P(500, PLAIN_TYPE, PSTR("Channel outsize"));
        }
    }

    static void page_handler(Req *r, web_server_file_t file) {
        switch (file) {
            case INDEX: return r->send_P(200, HTML_TYPE, index_html);
            case NOT_FOUND_HTML: return r->send_P(404, HTML_TYPE, not_found_html);
            case STYLES: return r->send_P(200, CSS_TYPE, styles_css);
            case APP_JS: return r->send_P(200, SCRIPT_TYPE, app_js);
        }
    }
    Core *c = nullptr;
    AsyncWebServer srv = AsyncWebServer(80);
};

#endif //ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
