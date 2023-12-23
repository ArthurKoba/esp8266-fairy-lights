#ifndef ESP8266_FAIRY_LIGHTS_CORE_H
#define ESP8266_FAIRY_LIGHTS_CORE_H

#include <Arduino.h>

#ifndef LED_PIN
    #define LED_PIN LED_BUILTIN
#endif
#ifndef BLINK_DELAY_MS
    #define BLINK_DELAY_MS 200
#endif


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

struct Packet {
    uint8_t packet_id;
    uint8_t data_id;
    uint16_t data_value;
};

class Core {
public:
    Core() {
        pinMode(LED_BUILTIN, OUTPUT);
    }

    uint32_t data_handler(const String &data) {
        if (data.length() != 4) return 0x4C45433F;
        Packet packet = *(Packet*)&data;
        Serial.print("\nLenght arg: ");
        Serial.println(data.length());
        Serial.print("packet_id : ");
        Serial.println(packet.packet_id);
        Serial.print("data_id: ");
        Serial.println(packet.data_id);
        Serial.print("data_value: ");
        Serial.println(packet.data_value);
        return 0x4F4B0000;
    }

    void blink() {
        if (millis() - last_blink < BLINK_DELAY_MS) return;
        last_blink = millis();
        states.led ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
        states.led = !states.led;
    }

private:
    uint32_t last_blink = 0;
    States states{};
};

#endif //ESP8266_FAIRY_LIGHTS_CORE_H
