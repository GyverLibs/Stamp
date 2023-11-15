#pragma once
#include <Arduino.h>

#include "Datime.h"
#include "StampUtils.h"
#include "stamp_zone.h"

class Stamp {
   public:
    uint32_t unix;

    // ========== CONSTRUCT ==========
    Stamp() {}
    Stamp(const Stamp& s) = default;
    Stamp(const char* str) {
        parse(str);
    }
    Stamp(uint32_t unix) {
        this->unix = unix;
    }
    Stamp(Datime& dt) {
        set(dt);
    }
    Stamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
        Datime dt(year, month, day, hour, minute, second);
        set(dt);
    }
    Stamp(uint16_t yh, uint16_t mm, uint16_t ds) {
        Datime dt(yh, mm, ds);
        set(dt);
    }

    // ============ TO STRING ============
    // вывести дату в формате "dd.mm.yyyy"
    char* dateToChar(char* buf) {
        Datime dt(getUnix());
        return dt.dateToChar(buf);
    }
    String dateToString() {
        Datime dt(getUnix());
        return dt.dateToString();
    }

    // вывести время в формате "hh:mm:ss"
    char* timeToChar(char* buf) {
        Datime dt(getUnix());
        return dt.timeToChar(buf);
    }
    String timeToString() {
        Datime dt(getUnix());
        return dt.timeToString();
    }

    char* toChar(char* buf, char div = ' ') {
        Datime dt(getUnix());
        return dt.toChar(buf, div);
    }
    String toString(char div = ' ') {
        Datime dt(getUnix());
        return dt.toString();
    }

    // =========== GET TIME ===========
    // экспортировать в формат Datime
    Datime get() {
        return Datime(getUnix());
    }

    // экспортировать в переменную типа Datime
    void get(Datime& dt) {
        dt.set(getUnix());
    }

    // =========== SET TIME ============
    // установить время из Datime
    void set(Datime& dt) {
        // #if _STAMP_ALG == STAMP_ALG_TIME_T
        // TODO time.h implementation
        // #else
        unix = dt.getUnix();
        // #endif
    }

    // установить время (год, месяц, день, час, минута, секунда)
    void set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
        Datime dt(year, month, day, hour, minute, second);
        set(dt);
    }

    // установить время (год, месяц, день) или (час, минута, секунда)
    void set(uint16_t yh, uint16_t mm, uint16_t ds) {
        Datime dt(yh, mm, ds);
        set(dt);
    }

    // =========== ADD ============
    // добавить секунды
    void addSeconds(uint32_t s) {
        unix += s;
    }

    // добавить минуты
    void addMinutes(uint32_t m) {
        unix += m * 60ul;
    }

    // добавить часы
    void addHours(uint32_t h) {
        unix += h * 3600ul;
    }

    // добавить дни
    void addDays(uint32_t d) {
        unix += d * 86400ul;
    }

    // =========== PARSE ============
    // hh:mm:ss или yyyy-mm-dd или yyyy-mm-ddThh:mm:ss
    bool parse(const char* s) {
        Datime dt;
        if (!dt.parse(s)) return 0;
        set(dt);
        return 1;
    }

    // <day_week>, dd <month> yyyy hh:mm:ss
    bool parseHTTP(const char* s) {
        Datime dt;
        if (!dt.parseHTTP(s)) return 0;
        set(dt);
        return 1;
    }

    // ============ EXPORT ============
    // получить секунды с epoch
    uint32_t toSeconds() {
        return getUnix();
    }

    // получить минуты с epoch
    uint32_t toMinutes() {
        return getUnix() / 60ul;
    }

    // получить часы с epoch
    uint32_t toHours() {
        return getUnix() / 3600ul;
    }

    // получить сутки с epoch
    uint32_t toDays() {
        return getUnix() / 86400ul;
    }

    // =========== OVERLOAD ===========
    // получить время в секундах
    virtual uint32_t getUnix() {
        return unix;
    }

    bool operator==(Stamp& dt) {
        return getUnix() == dt.getUnix();
    }
    bool operator>(Stamp& dt) {
        return getUnix() > dt.getUnix();
    }
    bool operator>=(Stamp& dt) {
        return getUnix() >= dt.getUnix();
    }
    bool operator<(Stamp& dt) {
        return getUnix() < dt.getUnix();
    }
    bool operator<=(Stamp& dt) {
        return getUnix() <= dt.getUnix();
    }

    void operator+=(uint32_t t) {
        unix += t;
    }
    void operator-=(uint32_t t) {
        unix -= t;
    }

    operator uint32_t() {
        return getUnix();
    }
    operator uint32_t*() {
        return &unix;
    }
};