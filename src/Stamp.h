#pragma once
#include <Arduino.h>
#include "Datime.h"
#include "zone.h"

#define DTIME_ALG 2

struct Stamp {
  uint32_t unix;

  // ========== CONSTRUCT ==========
  Stamp() {}
  /*Stamp(const char* str) {
    parse(str);
    }*/
  Stamp(uint32_t u) {
    unix = u;
  }
  Stamp(const Stamp& s) = default;
  Stamp(Datime& dt) {
    set(dt);
  }
  Stamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    Datime dt(year, month, day, hour, minute, second);
    set(dt);
  }
  Stamp(int yh, int mm, int ds) {
    Datime dt(yh, mm, ds);
    set(dt);
  }

  // ============ TO STRING ============
  // вывести дату в формате "dd.mm.yyyy"
  char* dateToChar(char* buf) {
    Datime dt;
    get(dt);
    return dt.dateToChar(buf);
  }
  String dateToString() {
    char buf[10];
    dateToChar(buf);
    return buf;
  }

  // вывести время в формате "hh:mm:ss"
  char* timeToChar(char* buf) {
    Datime dt;
    get(dt);
    return dt.timeToChar(buf);
  }
  String timeToString() {
    char buf[8];
    timeToChar(buf);
    return buf;
  }

  char* toChar(char* buf) {
    Datime dt;
    get(dt);
    return dt.toChar(buf);
  }
  String toString() {
    char buf[20];
    toChar(buf);
    return buf;
  }

  // =========== GET TIME ===========
  // экспортировать в формат Datime
  Datime get() {
    Datime dt;
    get(dt);
    return dt;
  }

  // экспортировать в переменную типа Datime
  void get(Datime& dt) {
    int zone = getStampZone();
    if (zone >= -12 && zone <= 14) zone *= 60;  // to minutes
    uint32_t u = getUnix() + zone * 60ul;

#if DTIME_ALG == 0
    dt.second = u % 60ul;
    u /= 60ul;
    dt.minute = u % 60ul;
    u /= 60ul;
    dt.hour = u % 24ul;
    u /= 24ul;
    dt.weekDay = (u + 4) % 7;
    if (!dt.weekDay) dt.weekDay = 7;
    uint32_t z = u + 719468;
    uint8_t era = z / 146097ul;
    uint16_t doe = z - era * 146097ul;
    uint16_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    uint16_t y = yoe + era * 400;
    uint16_t doy = doe - (yoe * 365 + yoe / 4 - yoe / 100);
    uint16_t mp = (doy * 5 + 2) / 153;
    dt.day = doy - (mp * 153 + 2) / 5 + 1;
    dt.month = mp + (mp < 10 ? 3 : -9);
    y += (dt.month <= 2);
    dt.year = y;

#elif DTIME_ALG == 1
    int32_t days, rem;
    int years400, years100, years4, remainingyears;

    days = u / 86400ul - 11017;
    rem = u % 86400ul;
    if (rem < 0) {
      rem += 86400ul;
      days--;
    }

    dt.hour = rem / 3600ul;
    rem %= 3600ul;
    dt.minute = rem / 60ul;
    dt.second = rem % 60ul;

    if ((dt.weekDay = ((3 + days) % 7)) < 0) dt.weekDay += 7;
    if (!dt.weekDay) dt.weekDay = 7;

    years400 = days / 146097L;
    days -= years400 * 146097L;
    if (days < 0) {
      days += 146097L;
      years400--;
    }

    years100 = days / 36524L;
    if (years100 == 4) years100--;
    days -= years100 * 36524L;
    years4 = days / 1461L;
    days -= years4 * 1461L;
    remainingyears = days / 365L;
    if (remainingyears == 4) remainingyears--;
    days -= remainingyears * 365L;
    dt.year = 2000 + years400 * 400 + years100 * 100 + years4 * 4 + remainingyears;
    bool yearleap = remainingyears == 0 && (years4 != 0 || years100 == 0);

    dt.yearDay = days + 31 + 28 + yearleap;
    if (dt.yearDay >= 365 + yearleap) {
      dt.yearDay -= 365 + yearleap;
      dt.year++;
    }
    dt.yearDay++;

    dt.month = 2;
    while (1) {
      int dm = ((dt.month < 7) ? ((dt.month == 1) ? 29 : ((dt.month & 1) ? 30 : 31)) : ((dt.month & 1) ? 31 : 30));
      if (days < dm) break;
      days -= dm;
      if (++dt.month >= 12) dt.month = 0;
    }
    dt.month++;
    dt.day = days + 1;

#elif DTIME_ALG == 2
    int32_t fract;
    uint16_t  days, n, leapyear, years;
    ldiv_t  lresult;
    div_t result;
    if (u < 946684800) u = 946684800;

    u -= 946684800;   // to 2000-01-01 UTC
    days = u / 86400UL;           // 38753+
    fract = u - days * 86400UL;   // 86400

    lresult = ldiv(fract, 60L);   // 86400
    dt.second = lresult.rem;
    result = div((int)lresult.quot, 60); // 1440
    dt.minute = result.rem;
    dt.hour = result.quot;

    n = days + 6;
    n %= 7;
    dt.wday = n;
    if (!dt.wday) dt.wday = 7;

    lresult = ldiv((long) days, 36525L);  // 38753+
    years = 100 * lresult.quot;
    lresult = ldiv(lresult.rem, 1461L);   // 36525
    years += 4 * lresult.quot;
    days = lresult.rem;       // <- 1461
    if (years > 100) days++;

    leapyear = 1;
    if (years == 100) leapyear = 0;
    n = 364 + leapyear;

    if (days > n) {
      days -= leapyear;
      leapyear = 0;
      result = div(days, 365);  // 1461
      years += result.quot;
      days = result.rem;     // <- 365
    }
    dt.year = 100 + years + 1900;
    dt.yday = days + 1;

    n = 59 + leapyear;
    if (days < n) {
      result = div(days, 31);  // 1461
      dt.month = result.quot;
      dt.day = result.rem;
    } else {
      days -= n;
      result = div(days, 153);  // 1461
      dt.month = 2 + result.quot * 5;
      result = div(result.rem, 61);  // 153
      dt.month += result.quot * 2;
      result = div(result.rem, 31);  // 61
      dt.month += result.quot;
      dt.day = result.rem;
    }

    dt.day++;
    dt.month++;
#endif
  }

  // =========== SET TIME ============
  // установить время из Datime
  void set(Datime& dt) {
    int zone = getStampZone();
    if (zone && zone >= -12 && zone <= 14) zone *= 60;  // to minutes
    int8_t my = (dt.month >= 3) ? 1 : 0;
    uint16_t y = dt.year + my - 1970;
    uint16_t dm = 0;
    for (int i = 0; i < dt.month - 1; i++) dm += ((i < 7) ? ((i == 1) ? 28 : ((i & 1) ? 30 : 31)) : ((i & 1) ? 31 : 30));
    unix = (((dt.day - 1 + dm + ((y + 1) >> 2) - ((y + 69) / 100) + ((y + 369) / 100 / 4) + 365 * (y - my)) * 24ul + dt.hour) * 60ul + dt.minute - zone) * 60ul + dt.second;
    if (unix < 946684800) unix = 946684800;
  }

  // установить время (год, месяц, день, час, минута, секунда)
  void set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    Datime dt(year, month, day, hour, minute, second);
    set(dt);
  }

  // установить время (год, месяц, день) или (час, минута, секунда)
  void set(int yh, int mm, int ds) {
    Datime dt(yh, mm, ds);
    set(dt);
  }

  /*
    // =========== PARSE ============
    // hh:mm:ss
    // yyyy-mm-dd
    // yyyy-mm-ddThh:mm:ss
    bool parse(const char* s) {
      Datime dt;
      if (!dt.parse(s)) return 0;
      set(dt);
      return 1;
    }

    // yyyy-mm-ddThh:mm:ss
    bool parseHTTP(const char* s) {
      Datime dt;
      if (!dt.parseHTTP(s)) return 0;
      set(dt);
      return 1;
    }
  */
  // ============ EXPORT ============
  virtual void _sync(bool force) {
  }

  // получить секунды
  uint32_t toSeconds() {
    return getUnix();
  }

  // получить минуты
  uint32_t toMinutes() {
    return getUnix() / 60ul;
  }

  // получить часы
  uint32_t toHours() {
    return getUnix() / 3600ul;
  }

  // получить сутки
  uint32_t toDays() {
    return getUnix() / 86400ul;
  }

  // ============ COMPARE ============
  /*Stamp diff(Stamp dt) {
    return (dt.getUnix() > getUnix()) ? (dt.getUnix() - getUnix()) : (getUnix() - dt.getUnix());
    }

    // ============== ADD ==============
    void addTime(Datime& dt) {
    unix += dt.toSeconds();
    }
    void add(uint32_t t) {
    unix += t;
    }*/

  // =========== OVERLOAD ===========
  // получить время в секундах
  uint32_t getUnix() {
    _sync(0);
    return unix;
  }

  bool operator == (Stamp& dt) {
    return getUnix() == dt.getUnix();
  }
  bool operator > (Stamp& dt) {
    return getUnix() > dt.getUnix();
  }
  bool operator >= (Stamp& dt) {
    return getUnix() >= dt.getUnix();
  }
  bool operator < (Stamp& dt) {
    return getUnix() < dt.getUnix();
  }
  bool operator <= (Stamp& dt) {
    return getUnix() <= dt.getUnix();
  }

  void operator += (uint32_t t) {
    unix += t;
  }
  void operator -= (uint32_t t) {
    unix -= t;
  }

  void operator += (Datime& dt) {
    unix += dt.toSeconds();
  }
  void operator -= (Datime& dt) {
    unix -= dt.toSeconds();
  }

  operator uint32_t() {
    return getUnix();
  }
};