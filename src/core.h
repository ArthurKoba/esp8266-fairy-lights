#ifndef ESP8266_FAIRY_LIGHTS_CORE_H
#define ESP8266_FAIRY_LIGHTS_CORE_H

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

#ifndef BLINK_DELAY_MS
#define BLINK_DELAY_MS 100
#endif

#ifndef UPDATE_CORE_DELAY
#define UPDATE_CORE_DELAY 5
#endif

#include "web_server_types.h"
#include "light_source.h"

typedef enum {
    INIT_SOURCES_OK = 0,
    INIT_SOURCES_FAIL_ALREADY_ADDED
} init_sources_status_t;

typedef enum {
    CHANGE_MODE_OK = 0,
    CHANGE_MODE_FAIL_OUTSIDE_SOURCE = 1,
    CHANGE_MODE_FAIL_OUTSIDE_MODE = 2
} change_mode_status_t;


class SourceItem {
public:
    explicit SourceItem(Source &_source) : next(nullptr), source(_source) {}
    SourceItem *next = nullptr;
    Source &source;
};


class Core {
public:
    Core() : first(nullptr), last(nullptr) {};
    ~Core() {
        SourceItem *p = first;
        SourceItem *deleted = nullptr;
        while (p) {
            deleted = p;
            p = p->next;
            delete deleted;
        };
    }

    init_sources_status_t add_light_source(Source &source) {
        auto* new_source_item_p = new SourceItem(source);
        if (first == nullptr) {
            first = new_source_item_p;
            last = new_source_item_p;
            return INIT_SOURCES_OK;
        }
        SourceItem *p = first;
        while (p) {
            if (&p->source == &source) return INIT_SOURCES_FAIL_ALREADY_ADDED;
            p = p->next;
        };
        last->next = new_source_item_p;
        last = new_source_item_p;
        return init_sources_status_t::INIT_SOURCES_OK;
    }

    void show_modes() {
        if (millis() - last_update < UPDATE_CORE_DELAY) return;
        SourceItem *p = first;
        if (!p) return;
        while (p) {
            p->source.show_mode();
            p = p->next;
        }
        last_update = millis();
    }

    void blink() {
        if (millis() - last_blink < BLINK_DELAY_MS) return;
        last_blink = millis();
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }

    size_t get_size_buffer_all_sources() {
        size_t count = 0;
        SourceItem *p = first;
        while (p) {
            count += p->source.get_name().length() + 2;
            p = p->next;
        }
        return count;
    }

    Source* get_source(uint8_t id) {
        SourceItem *p = first;
        uint8_t count = 0;
        while (p) {
            if (id == count++) return &p->source;
            p = p->next;
        }
        return nullptr;
    }

    void write_sources_info_to_buffer(uint8_t *buffer) {
        SourceItem *p = first;
        uint16_t offset = 0;
        while (p) {
            buffer[offset++] = p->source.get_mode();
            String &name = p->source.get_name();
            buffer[offset++] = name.length();
            for (int i = 0; i < name.length(); ++i) {buffer[offset++] = name.charAt(i);}
            p = p->next;
        }
    }

    change_mode_status_t change_mode(uint8_t source_id, uint8_t mode) {
        Source *source = get_source(source_id);
        if (!source) return change_mode_status_t::CHANGE_MODE_FAIL_OUTSIDE_SOURCE;
        if (mode >= ColorMode::UNKNOWN_MODE) return change_mode_status_t::CHANGE_MODE_FAIL_OUTSIDE_MODE;
        source->change_color_mode_source(static_cast<ColorMode>(mode));
        return change_mode_status_t::CHANGE_MODE_OK;
    }

private:
    SourceItem *first = nullptr;
    SourceItem *last = nullptr;
    uint32_t last_blink = 0;
    uint32_t last_update = 0;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
