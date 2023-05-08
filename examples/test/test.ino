#include "StampTicker.h"
#include <time.h>

void print_t(tm& t) {
  Serial.println(String() + (t.tm_year + 1900) + '.' + (t.tm_mon + 1) + '.' + (t.tm_mday) + ' ' + (t.tm_hour) + ':' + (t.tm_min) + ':' + (t.tm_sec));
}
void print_dt(Datime& t) {
  Serial.println(String() + (t.year) + '.' + (t.month) + '.' + (t.day) + ' ' + (t.hour) + ':' + (t.minute) + ':' + (t.second));
}

uint32_t us;
void b_begin() {
  us = micros();
}

void b_end(const char* s) {
  us = micros() - us;
  Serial.print(s);
  Serial.println(us);
  delay(50);
}

StampTicker dsync;

void setup() {
  Serial.begin(115200);
  setStampZone(3);
  Stamp ddd(2023, 5, 8);
  dsync.update(ddd);

  /*
    Stamp newd;
    newd.parse("2000-01-01");
    Serial.println(newd.toSeconds());
    newd.parse("12:35:24");
    Serial.println(newd.toString());
    newd.parse("2024-08-15T11:05:06");
    Serial.println(newd.toString());
    return;

    for (uint32_t i = 1681363859; i < 1681363859 + 86400 * 365; i += 1) {
      Stamp s(i);
      //Datime d = s.get();
      Datime d(15, 37, 55);
      if (s.getUnix() % 86400 == d.toSeconds()) Serial.println(s.toString());
    }
    Serial.println("end");
    return;


    Stamp d;
    d.parse("2022-03-12");
    Datime dtt;
    dtt.parse("12:35:00");
    d += dtt;
    Serial.println(d.toString());

    return;*/

  return;


  time_t t = 1681006324;
  tm tt;
  Stamp dt(1681006324);
  Datime dts;

  b_begin();
  gmtime_r(&t, &tt);
  b_end("gmtime_r: ");

  b_begin();
  t = mktime(&tt);
  b_end("mktime: ");

  b_begin();
  dt.get(dts);
  b_end("Stamp get: ");

  b_begin();
  dt.set(dts);
  b_end("Stamp set: ");

  print_t(tt);
  Serial.println(t);
  Serial.println(dt.toString());
  Serial.println(dts.toString());


  // unix to date test
  for (uint32_t i = 0; i < 365 * 100ul; i++) {
    uint32_t unix = 946684800 + 86400ul * i + random(86400);
    time_t t = unix - 946684800;
    tm tt;
    gmtime_r(&t, &tt);

    Stamp dt(unix);
    Datime dt_s = dt.get();

    if (dt_s.year != (tt.tm_year + 1900) ||
        dt_s.month != (tt.tm_mon + 1) ||
        dt_s.day != (tt.tm_mday ) ||
        dt_s.hour != (tt.tm_hour) ||
        dt_s.minute != (tt.tm_min) ||
        dt_s.second != (tt.tm_sec)
       ) {
      Serial.println();
      Serial.println(dt.toString());
      print_t(tt);
    }
  }

  // date to unix test
  for (uint32_t i = 0; i < 365 * 100ul; i++) {
    uint32_t unix = 946684800 + 86400ul * i + random(86400);
    time_t t = unix - 946684800;
    tm tt;
    gmtime_r(&t, &tt);

    Datime dt_s(tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
    Stamp dt(dt_s);

    if (dt.unix != unix) {
      Serial.println();
      Serial.println(dt.toString());
      print_t(tt);
    }
  }

  Serial.println("test end");
}

void loop() {
  delay(500);
  Serial.println(dsync.toString());
}