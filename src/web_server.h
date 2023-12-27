#ifndef ESP8266_FAIRY_LIGHTS_WEB_SERVER_H
#define ESP8266_FAIRY_LIGHTS_WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "core_types.h"
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
        srv.on("/updateChannel", HTTP_POST,
               [] (Req *r) {},
               nullptr,
               [this] (Req *r, const uint8_t *data, size_t len, size_t index, size_t total) {
            update_channel_handler(r, data, len);
        });
        srv.on("/getChannelsBright", HTTP_GET, [this] (Req *r) { get_channels(r);});
        srv.on("/", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::INDEX);});
        srv.on("/styles.css", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::STYLES);});
        srv.on("/app.js", HTTP_GET, [this] (Req *r) {page_handler(r, web_server_file_t::APP_JS);});
        srv.onNotFound( [this] (Req *r) {page_handler(r, web_server_file_t::NOT_FOUND_HTML);});

        srv.begin();
    }

protected:
    void get_channels(Req *r) {
        auto *brights = new uint8_t[c->get_len_channels()];
        c->write_channels_bright_to_buffer(brights);
        r->send_P(200, OCTET_TYPE, brights, c->get_len_channels());
        delete[] brights;
    }

    void update_channel_handler(Req *r, const uint8_t *data, size_t len) {
        if (sizeof(UpdateChannelPacket) != len) {
            return r->send_P(406, PLAIN_TYPE, PSTR("Packet length error"));
        }
        UpdateChannelPacket packet = *(UpdateChannelPacket*)data;
        write_channel_status_t status = c->write_channel(packet.channel, packet.bright);
        switch (status) {
            case CHANNEL_WRITE_OK: r->send_P(200, PLAIN_TYPE, PSTR("OK")); break;
            case CHANNEL_OUTSIDE: r->send_P(500, PLAIN_TYPE, PSTR("Channel beyond initialization channels")); break;
            case CHANNEL_NOT_INITED: r->send_P(500, PLAIN_TYPE, PSTR("Channel not inited if firmware")); break;
            case NOT_INITED_CHANNELS: r->send_P(500, PLAIN_TYPE, PSTR("Firmware error. Channels not inited in core.init()")); break;
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
