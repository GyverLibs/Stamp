#pragma once
#include <Arduino.h>

class DaySeconds {
   public:
    DaySeconds() {}
    DaySeconds(uint8_t hour, uint8_t minute, uint8_t second) {
        set(hour, minute, second);
    }

    void set(uint8_t hour, uint8_t minute, uint8_t second) {
        seconds = hour * 3600ul + minute * 60 + second;
    }

    uint32_t seconds = 0;
};