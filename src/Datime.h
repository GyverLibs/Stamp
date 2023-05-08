#pragma once
#include <Arduino.h>

struct Datime {
  // год
  union {
    uint16_t year = 2000;
    uint16_t y;
  };

  // месяц (1.. 12)
  union {
    uint8_t month = 1;
    uint8_t mon;
    uint8_t mo;
  };

  // день (1.. 28-31)
  union {
    uint8_t day = 1;
    uint8_t mday;
    uint8_t d;
  };

  // час (0.. 23)
  union {
    uint8_t hour = 0;
    uint8_t h;
  };

  // минута (0.. 59)
  union {
    uint8_t minute = 0;
    uint8_t min;
    uint8_t m;
  };

  // секунда (0.. 59)
  union {
    uint8_t second = 0;
    uint8_t sec;
    uint8_t s;
  };

  // день недели (1 пн.. 7 вс)
  union {
    uint8_t weekDay = 0;
    uint8_t wday;
    uint8_t wd;
  };

  // день года (1.. 365-366)
  union {
    uint16_t yearDay = 0;
    uint16_t yday;
    uint16_t yd;
  };

  // ========= CONSTRUCTOR =========
  Datime() {}
  /*Datime(const char* str) {
    parse(str);
    }*/
  Datime(uint16_t nyear, uint8_t nmonth, uint8_t nday, uint8_t nhour, uint8_t nminute, uint8_t nsecond) {
    set(nyear, nmonth, nday, nhour, nminute, nsecond);
  }
  Datime(int yh, int mm, int ds) {
    set(yh, mm, ds);
  }
  Datime(const Datime& dat) = default;

  // ============= SET =============
  // установить время (год, месяц, день, час, минута, секунда)
  void set(uint16_t nyear, uint8_t nmonth, uint8_t nday, uint8_t nhour, uint8_t nminute, uint8_t nsecond) {
    year = nyear;
    month = nmonth;
    day = nday;
    hour = nhour;
    minute = nminute;
    second = nsecond;
  }

  // установить время (год, месяц, день) или (час, минута, секунда)
  void set(int yh, int mm, int ds) {
    if (yh > 60) {
      year = yh;
      month = mm;
      day = ds;
    } else {
      hour = yh;
      minute = mm;
      second = ds;
    }
  }

  // =========== EXPORT ============
  // вывести в секунды
  uint32_t toSeconds() {
    return hour * 3600ul + minute * 60 + second;
  }

  // ========== TO STRING ==========
  // вывести дату в формате "dd.mm.yyyy"
  char* dateToChar(char* buf) {
    buf[0] = day / 10 + '0';
    buf[1] = day % 10 + '0';
    buf[2] = '.';
    buf[3] = month / 10 + '0';
    buf[4] = month % 10 + '0';
    buf[5] = '.';
    itoa(year, buf + 6, DEC);
    return buf + 10;
  }

  // вывести дату в формате "dd.mm.yyyy"
  String dateToString() {
    char buf[10];
    dateToChar(buf);
    return buf;
  }

  // вывести время в формате "hh:mm:ss"
  char* timeToChar(char* buf) {
    buf[0] = hour / 10 + '0';
    buf[1] = hour % 10 + '0';
    buf[2] = ':';
    buf[3] = minute / 10 + '0';
    buf[4] = minute % 10 + '0';
    buf[5] = ':';
    buf[6] = second / 10 + '0';
    buf[7] = second % 10 + '0';
    buf[8] = 0;
    return buf + 8;
  }

  // вывести время в формате "hh:mm:ss"
  String timeToString() {
    char buf[8];
    timeToChar(buf);
    return buf;
  }

  // вывести в формате dd.mm.yyyy hh:mm:ss
  char* toChar(char* buf) {
    char* s = buf;
    s = dateToChar(s);
    s[0] = ' ';
    s = timeToChar(s + 1);
    return s;
  }
  String toString() {
    char buf[20];
    toChar(buf);
    return buf;
  }

  /*
    // yyyy-mm-ddThh:mm:ss
    char* toChar(char* buf, char div = 'T') {
      char* s = buf;
      s = dateToChar(s);
      s[0] = div;
      s = timeToChar(s + 1);
      return s;
    }
    String toString(char div = 'T') {
      char buf[20];
      toChar(buf, div);
      return buf;
    }

    // ========== PARSE ==========
    // yyyy-mm-dd
    bool parseDate(const char* s) {
      if (strlen(s) < 10) return 0;
      year = atoi(s);
      s = strchr(s, '-');
      if (!s) return 0;
      month = atoi(++s);
      s = strchr(s, '-');
      if (!s) return 0;
      day = atoi(++s);
      return 1;
    }

    // hh:mm:ss
    bool parseTime(const char* s) {
      if (strlen(s) < 8) return 0;
      hour = atoi(s);
      s = strchr(s, ':');
      if (!s) return 0;
      minute = atoi(++s);
      s = strchr(s, ':');
      if (!s) return 0;
      second = atoi(++s);
      return 1;
    }

    // yyyy-mm-dd
    // hh:mm:ss
    // yyyy-mm-ddThh:mm:ss
    bool parse(const char* s) {
      uint16_t len = strlen(s);
      if (len == 19 && s[10] == 'T') {  // dateTtime
        if (!parseDate(s)) return 0;
        if (!parseTime(s + 11)) return 0;
      } else if (len == 10) { // date
        if (!parseDate(s)) return 0;
      } else if (len == 8) {  // time
        if (!parseTime(s)) return 0;
      } else {
        return 0;
      }
      return 1;
    }

    // <day_week>, dd <month> yyyy hh:mm:ss
    bool parseHTTP(const char* s) {
      char* comma = strchr(s, ',');
      if (!comma) return 0;
      s = comma + 2;
      day = atoi(s);
      month = 0;
      switch (s[3]) {
        case 'J': month = (s[4] == 'a') ? 1 : ((s[5] == 'n') ? 6 : 7); break;
        case 'F': month = 2; break;
        case 'A': month = (s[5] == 'r') ? 4 : 8; break;
        case 'M': month = (s[5] == 'r') ? 3 : 5; break;
        case 'S': month = 9; break;
        case 'O': month = 10; break;
        case 'N': month = 11; break;
        case 'D': month = 12; break;
        default: return 0;
      }
      year = atoi(s + 7);
      hour = atoi(s + 12);
      minute = atoi(s + 15);
      second = atoi(s + 18);
      return 1;
    }
  */
};