#pragma once
#include <Arduino.h>

#include "Stamp.h"

class StampSync : public Stamp {
   public:
    StampSync() {}

    // установить текущий unix, дополнительно миллисекунды синхронизации
    StampSync(uint32_t unix, uint16_t ms = 0) {
        update(unix, ms);
    }

    // установить текущий unix, дополнительно миллисекунды синхронизации
    void update(uint32_t unix, uint16_t ms = 0) {
        this->unix = unix;
        _updTime = millis() - ms;
    }

    // время синхронизировано
    bool synced() {
        return unix;
    }

    // получить текущий unix
    uint32_t getUnix() {
        if (!synced()) return 0;
        uint32_t ms = millis();
        uint32_t diff = ms - _updTime;
        if (diff > 86400000ul) {  // 24h
            unix += diff / 1000ul;
            _updTime = ms - diff % 1000ul;
            diff = 0;
        }
        return unix + diff / 1000ul;
    }

    // получить миллисекунды текущей секунды
    uint16_t ms() {
        return (millis() - _updTime) % 1000ul;
    }

   private:
    uint32_t _updTime = 0;
};