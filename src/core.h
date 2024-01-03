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

typedef enum {
    CHANGE_BRIGHT_OK = 0,
    CHANGE_BRIGHT_FAIL_OUTSIDE_SOURCE = 1,
    CHANGE_BRIGHT_FAIL_OUTSIDE_CHANNEL = 2
} change_bright_status_t;


class SourceItem {
public:
    explicit SourceItem(Source &_source) : next(nullptr), source(_source) {}
    SourceItem *next = nullptr;
    Source &source;
};


class Core {
public:
    explicit Core(uint8_t led_pin_ = 255) : first(nullptr), last(nullptr) {
        led_pin = led_pin_;
        pinMode(led_pin, OUTPUT);
        analogWriteResolution(10);
    };

    ~Core() {
        SourceItem *p = first;
        SourceItem *deleted;
        while (p) {
            deleted = p;
            p = p->next;
            delete deleted;
        };
    }

    init_sources_status_t add_light_source(Source &source) {
        auto* new_source_item_p = new SourceItem(source);
        count_sources++;
        count_chars_info_all_channels += source.get_count_chars_channels_info();
        count_all_channels += source.get_count_channels();
        count_chars_info_all_sources += source.get_name().length();
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
        if (millis() - last_blink < BLINK_DELAY_MS || led_pin == 255) return;
        last_blink = millis();
        digitalWrite(led_pin, !digitalRead(led_pin));
    }

    size_t get_source_info_buffer_length() const {
        // count sources (byte) + sources * 2 (modes + channels) + channels * 2 (bright and len chars) + chars
        return 1 + count_sources * 3 + count_chars_info_all_sources + count_all_channels * 2 + count_chars_info_all_channels;
    }

    size_t get_sources_info(uint8_t *buffer) {
        if (!count_sources) return 0;
        size_t size = 0;
        buffer[size++] = count_sources; // set count sources
        SourceItem *p = first;
        while (p) {
            Source *source = &p->source;
            buffer[size++] = source->get_mode(); // set source mode
            buffer[size++] = source->get_name().length(); // set source mode
            for (int i = 0; i < source->get_name().length(); ++i) { // write source name
                buffer[size++] = source->get_name().c_str()[i];
            }
            buffer[size++] = source->get_count_channels(); // set count channels in source
            for (int i = 0; i < source->get_count_channels(); ++i) {
                Channel *ch = source->get_channel(i);
                buffer[size++] = ch->bright; // set bright
                buffer[size++] = ch->name.length(); // set count chars in name channel
                for (int j = 0; j < ch->name.length(); ++j) {
                    buffer[size++] = ch->name.c_str()[j]; // write name
                }
            }
            p = p->next;
        }
        return size;
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

    change_mode_status_t change_mode(uint8_t source_id, uint8_t mode) {
        Source *source = get_source(source_id);
        if (!source) return change_mode_status_t::CHANGE_MODE_FAIL_OUTSIDE_SOURCE;
        if (mode >= ColorMode::UNKNOWN_MODE) return change_mode_status_t::CHANGE_MODE_FAIL_OUTSIDE_MODE;
        source->set_mode(static_cast<ColorMode>(mode));
        return change_mode_status_t::CHANGE_MODE_OK;
    }

    change_bright_status_t change_bright(uint8_t source_id, uint8_t channel_id, uint8_t bright) {
        Source *source = get_source(source_id);
        if (!source) return change_bright_status_t::CHANGE_BRIGHT_FAIL_OUTSIDE_SOURCE;
        if (!source->set_bright(channel_id, bright))
            return change_bright_status_t::CHANGE_BRIGHT_FAIL_OUTSIDE_CHANNEL;
        source->set_mode(ColorMode::COLOR_MODE);
        return change_bright_status_t::CHANGE_BRIGHT_OK;
    };

private:
    SourceItem *first = nullptr;
    SourceItem *last = nullptr;
    uint32_t last_blink = 0;
    uint32_t last_update = 0;
    uint16_t count_chars_info_all_channels = 0;
    uint16_t count_chars_info_all_sources = 0;
    uint8_t count_all_channels = 0;
    uint8_t count_sources = 0;
    uint8_t led_pin = 255;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
