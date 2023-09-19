#pragma once
#include <Arduino.h>

#include "Stamp.h"

class StampTicker : public Stamp {
  public:
    StampTicker() {
      unix = 0;
    }

    // установить текущий unix, дополнительно миллисекунды
    void update(uint32_t nunix, uint16_t ms = 0) {
      unix = nunix;
      _updTime = millis() - ms;
    }

    // время синхронизировано
    bool synced() {
      return unix;
    }

    // получить миллисекунды текущей секунды
    uint16_t ms() {
      return (millis() - _updTime) % 1000;
    }

    uint32_t getUnix() {
      // защита от переполнения разности через 50 суток
      uint32_t ms = millis();
      uint32_t diff = ms - _updTime;
      if (diff > 86400000ul) {  // 24h
        unix += diff / 1000ul;
        _updTime = ms - diff % 1000ul;
        diff = 0;
      }
      return unix + diff / 1000ul;
    }

  private:
    uint32_t _updTime = 0;
};

/*
// backup
class StampTicker : public Stamp {
   public:
    // установить текущий unix, дополнительно миллисекунды
    void update(uint32_t nunix, uint16_t ms = 0) {
        _unixBuf = nunix;
        _updTime = millis() - ms;
        _sync(true);
    }

    // время синхронизировано
    bool synced() {
        return _unixBuf;
    }

    // получить миллисекунды текущей секунды
    uint16_t ms() {
        return (millis() - _updTime) % 1000;
    }

   private:
    void _sync(bool force = 0) {
        if (!synced()) return;

        uint32_t ms = millis();
        if (!force && (uint16_t)((uint16_t)ms - _secTmr) < 900) return;
        _secTmr = ms;

        // защита от переполнения разности через 50 суток
        uint32_t diff = ms - _updTime;
        if (diff > 86400000ul) {  // 24h
            _unixBuf += diff / 1000ul;
            _updTime = ms - diff % 1000ul;
            diff = 0;
        }
        unix = _unixBuf + diff / 1000ul;
    }

    uint32_t _updTime = 0;
    uint32_t _unixBuf = 0;  // sync
    uint16_t _secTmr = 0;
};
*/