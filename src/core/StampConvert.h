#pragma once
#include <Arduino.h>

#include "../Datime.h"
#include "../DaySeconds.h"

class StampConvert {
   public:
    virtual uint32_t getUnix() = 0;

    // =========== GET TIME ===========
    // экспортировать в локальное время Datime
    Datime now() {
        return getUnix();
    }

    operator Datime() {
        return getUnix();
    }

    // ============ TO STRING ============
    // вывести дату в формате "dd.mm.yyyy"
    char* dateToChar(char* buf) {
        return now().dateToChar(buf);
    }
    String dateToString() {
        return now().dateToString();
    }

    // вывести время в формате "hh:mm:ss"
    char* timeToChar(char* buf) {
        return now().timeToChar(buf);
    }
    String timeToString() {
        return now().timeToString();
    }

    // вывести время и дату в формате "dd.mm.yyyy hh:mm:ss"
    char* toChar(char* buf, char div = ' ') {
        return now().toChar(buf, div);
    }
    String toString(char div = ' ') {
        return now().toString();
    }

    // ============ EXPORT EPOCH ============

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

    // ============ DATIME ============

    // получить секунды с начала текущих суток (локальное время)
    uint32_t daySeconds() {
        return _localUnix() % 86400;
    }

    // получить текущие секунды
    uint8_t second() {
        return _localUnix() % 60ul;
    }

    // получить текущие минуты
    uint8_t minute() {
        return (_localUnix() / 60ul) % 60ul;
    }

    // получить текущие часы
    uint8_t hour() {
        return (_localUnix() / 3600ul) % 24ul;
    }

    // получить текущий день месяца
    uint8_t day() {
        return now().day;
    }

    // получить текущий месяц
    uint8_t month() {
        return now().month;
    }

    // получить текущий год
    uint16_t year() {
        return now().year;
    }

    // получить текущий день недели
    uint8_t weekDay() {
        return now().weekDay;
    }

    // получить текущий день года
    uint8_t yearDay() {
        return now().yearDay;
    }

    // =========== OVERLOAD ===========
    bool operator==(const Datime& dt) {
        return getUnix() == dt.getUnix();
    }
    bool operator!=(const Datime& dt) {
        return getUnix() != dt.getUnix();
    }
    bool operator>(const Datime& dt) {
        return getUnix() > dt.getUnix();
    }
    bool operator>=(const Datime& dt) {
        return getUnix() >= dt.getUnix();
    }
    bool operator<(const Datime& dt) {
        return getUnix() < dt.getUnix();
    }
    bool operator<=(const Datime& dt) {
        return getUnix() <= dt.getUnix();
    }
    
    bool operator==(StampConvert& s) {
        return getUnix() == s.getUnix();
    }
    bool operator!=(StampConvert& s) {
        return getUnix() != s.getUnix();
    }
    bool operator>(StampConvert& s) {
        return getUnix() > s.getUnix();
    }
    bool operator>=(StampConvert& s) {
        return getUnix() >= s.getUnix();
    }
    bool operator<(StampConvert& s) {
        return getUnix() < s.getUnix();
    }
    bool operator<=(StampConvert& s) {
        return getUnix() <= s.getUnix();
    }

    bool operator==(uint32_t u) {
        return getUnix() == u;
    }
    bool operator!=(uint32_t u) {
        return getUnix() != u;
    }
    bool operator>(uint32_t u) {
        return getUnix() > u;
    }
    bool operator>=(uint32_t u) {
        return getUnix() >= u;
    }
    bool operator<(uint32_t u) {
        return getUnix() < u;
    }
    bool operator<=(uint32_t u) {
        return getUnix() <= u;
    }

    bool operator==(const DaySeconds& ds) {
        return daySeconds() == ds.seconds;
    }
    bool operator!=(const DaySeconds& ds) {
        return daySeconds() != ds.seconds;
    }
    bool operator>(const DaySeconds& ds) {
        return daySeconds() > ds.seconds;
    }
    bool operator>=(const DaySeconds& ds) {
        return daySeconds() >= ds.seconds;
    }
    bool operator<(const DaySeconds& ds) {
        return daySeconds() < ds.seconds;
    }
    bool operator<=(const DaySeconds& ds) {
        return daySeconds() <= ds.seconds;
    }

    // экспортировать в локальное время Datime
    Datime get() {
        return getUnix();
    }

    // экспортировать в переменную типа Datime
    void get(Datime& dt) {
        dt = getUnix();
    }

   private:
    uint32_t _localUnix() {
        return getUnix() + getStampZone() * 60l;
    }
};