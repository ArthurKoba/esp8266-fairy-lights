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

#ifndef CH1_PIN
    #error CH1_PIN not defined
#endif

#ifndef CH2_PIN
    #error CH2_PIN not defined
#endif

#ifndef CH3_PIN
    #error CH3_PIN not defined
#endif

#ifndef CH4_PIN
    #error CH4_PIN not defined
#endif

#ifndef CH5_PIN
    #error CH5_PIN not defined
#endif

enum Channels {
    ONE = CH1_PIN, TWO = CH2_PIN, THREE = CH3_PIN, FOUR = CH4_PIN, FIVE = CH5_PIN
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

struct UpdatePacket {
    uint8_t channel;
    uint8_t bright;
};

class Core {
public:
    Core() {
        pinMode(LED_PIN, OUTPUT);
        pinMode(CH1_PIN, OUTPUT);
        pinMode(CH2_PIN, OUTPUT);
        pinMode(CH3_PIN, OUTPUT);
        pinMode(CH4_PIN, OUTPUT);
        pinMode(CH5_PIN, OUTPUT);
    }

    void data_handler(const uint8_t *data, size_t len) {
        uint8_t payload_ptr = *(data + 1);
        if (data[0] == 50) {
            UpdatePacket packet = *(UpdatePacket*)&payload_ptr;
            switch (packet.channel)  {
                case 1: case 2: case 3: case 4: case 5: write_bright(packet.channel, packet.bright); break;
            }
        }
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

private:
    static void write_bright(uint8_t pin, uint8_t value) {
        float val = value;
        val = (float)((val > 23) ? (0.00512 * val * val - 0.325 * val + 5.73) : 0);
        analogWrite(pin, (uint8_t)val);
    }

    uint32_t last_blink = 0;
    uint32_t last_fast_blink = 0;
    States states{};
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
