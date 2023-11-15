#pragma once
#include <Arduino.h>

#include "Stamp.h"

class StampTicker : public Stamp {
   public:
    StampTicker() {}

    // установить текущий unix, дополнительно миллисекунды синхронизации
    StampTicker(uint32_t unix, uint16_t ms = 0) {
        update(unix, ms);
    }

    // установить текущий unix, дополнительно миллисекунды синхронизации
    void update(uint32_t unix, uint16_t ms = 0) {
        this->unix = _syncUnix = unix;
        _updTime = millis() - ms;
        _secTmr = _updTime;
    }

    // подключить функцию-обработчик срабатывания (вида void f())
    void attach(void (*handler)()) {
        _cb = *handler;
    }

    // отключить функцию-обработчик срабатывания
    void detach() {
        _cb = nullptr;
    }

    // тикер, вызывать в loop. Обновляет unix раз в секунду. Вернёт true каждую секунду с учётом мс синхронизации
    bool tick() {
        _ready = 0;
        uint16_t timeLeft = (uint16_t)millis() - _secTmr;
        if (timeLeft >= 1000) {
            _secTmr += ((timeLeft >= 2 * 1000) ? (timeLeft / 1000) : 1) * 1000;
            _ready = 1;
            unix = calcUnix();
            if (_cb) _cb();
        }
        return _ready;
    }

    // возвращает true каждую секунду
    bool ready() {
        return _ready;
    }

    // время синхронизировано
    bool synced() {
        return _syncUnix;
    }

    // получить миллисекунды текущей секунды
    uint16_t ms() {
        uint16_t m = (uint16_t)millis() - _secTmr;
        return (m >= 1000) ? ((millis() - _updTime) % 1000ul) : m;  // если не вызывался тикер
    }

    // вычислить и получить текущий unix
    uint32_t calcUnix() {
        if (!synced()) return 0;
        uint32_t ms = millis();
        uint32_t diff = ms - _updTime;
        if (diff > 86400000ul) {  // 24h
            _syncUnix += diff / 1000ul;
            _updTime = ms - diff % 1000ul;
            diff = 0;
        }
        return _syncUnix + diff / 1000ul;
    }

    operator uint32_t*() {
        return &unix;
    }

   private:
    void (*_cb)() = nullptr;
    uint32_t _syncUnix = 0;
    uint32_t _updTime = 0;
    uint16_t _secTmr = 0;
    bool _ready = 0;
};