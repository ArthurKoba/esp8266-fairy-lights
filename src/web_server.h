#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <ESPAsyncTCP.h>
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
        srv.on("/changeMode", HTTP_POST, [] (Req *r) {},nullptr,
       [this] (Req *r, const uint8_t *d, size_t l, size_t i, size_t t) {change_mode(r, d, l);
        });
        srv.on("/getSources", HTTP_GET, [this] (Req *r) { get_sources(r);});
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

    void get_sources(Req *r) {
        size_t buffer_size = c->get_size_buffer_all_sources();
        auto *data = new uint8_t[buffer_size];
        c->write_sources_info_to_buffer(data);
        r->send_P(200, OCTET_TYPE, data, buffer_size);
        delete[] data;
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
