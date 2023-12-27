#ifndef ESP8266_FAIRY_LIGHTS_CORE_H
#define ESP8266_FAIRY_LIGHTS_CORE_H

#include <Arduino.h>

#ifndef LED_PIN
    #define LED_PIN LED_BUILTIN
#endif

#ifndef BLINK_DELAY_MS
    #define BLINK_DELAY_MS 2000
#endif

#ifndef FAST_BLINK_DELAY_MS
    #define FAST_BLINK_DELAY_MS 2000
#endif

#ifndef UPDATE_DELAY
    #define UPDATE_DELAY 5
#endif

enum InitChannelsStatus : uint8_t {INIT_OK = 0, ERROR_LENGTH = 1};

enum DataProcessingStatus : uint8_t {
    SUCCESS = 0,
    SYSTEM_ERROR,
    UNKNOWN_PACKET,
    PACKET_LENGTH_ERROR,
    CH_NOT_INITED, // channel is not initialized
    CH_OUTSIDE, // channel beyond available
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

struct __attribute__((__packed__)) UpdateChannelPacket {
    uint8_t channel;
    uint8_t bright;
};

class Core {
public:
    Core() = default;

    DataProcessingStatus data_handler(const uint8_t *data, size_t len) {
        uint8_t packed_id = data[0];
        switch (packed_id) {
            case 50:
                if (sizeof(UpdateChannelPacket) != len -1) return PACKET_LENGTH_ERROR;
                UpdateChannelPacket packet = *(UpdateChannelPacket*)&*(data+1);
                return write_channel(packet.channel, packet.bright);
                break;
        }
        return UNKNOWN_PACKET;
    }

    DataProcessingStatus write_channel(uint8_t channel, uint8_t val) {
        if (channel > channels_length) return CH_OUTSIDE;
        if (channels == nullptr) return SYSTEM_ERROR;
        Channel &ch = channels[channel];
        if (ch.is_not_inited) return CH_NOT_INITED;
        ch.bright = val;
//        write_bright_CRT(ch);
        ch.is_need_update = true;
        return SUCCESS;
    }

    InitChannelsStatus init_channels(Channel *channels_ptr, uint8_t len) {
        if (len < 1) return InitChannelsStatus::ERROR_LENGTH;
        channels = channels_ptr;
        channels_length = len;
        for (int i = 0; i < len; ++i) {
            if (channels[i].pin < 0) continue;
            pinMode(channels[i].pin, OUTPUT);
            channels[i].is_not_inited = false;
            write_bright_CRT(channels[i]);
        }
        return InitChannelsStatus::INIT_OK;
    }

    static void write_bright_CRT(Channel &channel) {
        uint8_t val = channel.bright;
        val = ((long)val * val + 255) >> 8;
        analogWrite(channel.pin, val);
        channel.is_need_update = false;
    }

    void blink() {
        if (millis() - last_blink < BLINK_DELAY_MS) return;
        last_blink = millis();
        states.led ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
        states.led = !states.led;
    }
    void fast_blink() {
        if (millis() - last_fast_blink < FAST_BLINK_DELAY_MS) return;
        last_fast_blink = millis();
        states.led ? digitalWrite(LED_PIN, HIGH) : digitalWrite(LED_PIN, LOW);
        states.led = !states.led;
    }

    void update() {
        if (millis() - last_update < UPDATE_DELAY) return;
        last_update = millis();
        for (int i = 0; i < channels_length; ++i) {
            Channel &ch = channels[i];
            if (!ch.is_need_update) continue;
            Serial.println(ch.is_need_update);
            write_bright_CRT(ch);
        }
    }

private:
    Channel *channels = nullptr;
    uint8_t channels_length = 0;
    States states{};
    uint32_t last_blink = 0;
    uint32_t last_fast_blink = 0;
    uint32_t last_update = 0;
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
