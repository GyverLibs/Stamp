#pragma once
#include <Arduino.h>
#include "Stamp.h"

struct StampTicker : public Stamp {
  // установить текущий unix
  void update(uint32_t dt) {
    _unixBuf = dt;
    _updTime = millis();
    _sync(true);
  }

  // время синхронизировано
  bool synced() {
    return _unixBuf;
  }

  void _sync(bool force = 0) {
    if (!synced()) return;

    uint32_t ms = millis();
    if (!force && (uint16_t)ms - _secTmr < 900) return;
    _secTmr = ms;

    // защита от переполнения разности через 50 суток
    uint32_t diff = ms - _updTime;
    if (diff > 86400000ul) {    // 24h
      _unixBuf += diff / 1000ul;
      _updTime = ms - diff % 1000ul;
    }
    unix = _unixBuf + diff / 1000ul;
  }

  uint32_t _updTime = 0;
  uint32_t _unixBuf = 0;   // sync
  uint16_t _secTmr = 0;
};