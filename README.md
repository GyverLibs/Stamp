[![latest](https://img.shields.io/github/v/release/GyverLibs/Stamp.svg?color=brightgreen)](https://github.com/GyverLibs/Stamp/releases/latest/download/Stamp.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Stamp?_x_tr_sl=ru&_x_tr_tl=en)

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Stamp
Библиотека для хранения и манипуляции со временем
- Более удобная в использовании, чем встроенная time.h
- Используются более быстрые алгоритмы преобразования, чем в time.h
- Хранит дату и время в 4 байт переменной (секунды с 01.01.2000)
- Преобразует в год, месяц, день, часы, минуты, секунды, день недели, день года
- Может считать и поддерживать время на базе millis()
- Парсинг из строк
- Работает ~до 2106 года

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Документация](#reference)
- [Пример](#example)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="reference"></a>

## Документация
### TimeZone
```cpp
void setStampZone(int zone);    // установить глобальную часовую зону в часах или минутах
int getStampZone();             // получить глобальную часовую зону
```

### Datime
Хранит дату и время в отдельных переменных:
```cpp
uint16_t year;      // год
uint8_t month;      // месяц (1.. 12)
uint8_t day;        // день месяца (1.. 28-31)
uint8_t hour;       // час (0.. 23)
uint8_t minute;     // минута (0.. 59)
uint8_t second;     // секунда (0.. 59)
uint8_t weekDay;    // день недели (1 пн.. 7 вс) ISO 8601
uint16_t yearDay;   // день года (1.. 365-366)

// указанные диапазоны справедливы для всех остальных функций библиотеки!
```

```cpp
// ============ КОНСТРУКТОР ============
Datime;
Datime(unix);                       // unix время
Datime(year, month, day);           // год, месяц, день
Datime(hour, minute, second);       // час, минута, секунда
Datime(year, month, day, hour, minute, second);
Datime(const char* str);            // строка вида yyyy-mm-ddThh:mm:ss

// ============= УСТАНОВКА =============
void set(unix);                     // unix время
void set(year, month, day);         // год, месяц, день
void set(hour, minute, second);     // час, минута, секунда
void set(year, month, day, hour, minute, second);

// ============= ПАРСИНГ ==============
bool parseDate(const char* s);      // из строки вида yyyy-mm-dd
bool parseTime(const char* s);      // из строки вида hh:mm:ss
bool parse(const char* s);          // из строки вида yyyy-mm-dd или hh:mm:ss или yyyy-mm-ddThh:mm:ss
bool parseHTTP(const char* s);      // из строки вида <day_week>, dd <month> yyyy hh:mm:ss

// ============== ЭКСПОРТ ==============
uint32_t toSeconds();               // вывести время в секунды (без учёта даты)
uint32_t getUnix();                 // вывести в unix-секунды

// ============= В СТРОКУ ==============
// вывести дату в формате "dd.mm.yyyy"
char* dateToChar(char* buf);
String dateToString();

// вывести время в формате "hh:mm:ss"
char* timeToChar(char* buf);
String timeToString();

// вывести в формате dd.mm.yyyy hh:mm:ss, div - разделитель
char* toChar(char* buf, char div = ' ');
String toString(char div = ' ');

// ============= ДОБАВИТЬ ==============
void addSeconds(uint32_t s);    // добавить секунды
void addMinutes(uint32_t m);    // добавить минуты
void addHours(uint32_t h);      // добавить часы
void addDays(uint32_t d);       // добавить дни

void nextSecond();              // следующая секунда
void nextMinute();              // следующая минута (xx:xx:00)
void nextHour();                // следующий час (xx:00:00)
void nextDay();                 // следующий день (00:00:00)
void nextMonth();               // следующий месяц (1 число 00:00:00)

void updateDays();              // обновить weekDay и yearDay исходя из текущей даты (после ручного изменения)

// ============= АЛГОРИТМ =============
// Алгоритм преобразования времени задаётся перед подключением библиотеки
// для исследования и просто так чтобы было
#define UNIX_ALG UNIX_ALG_0       // ~402us и ~94B Flash (AVR)
#define UNIX_ALG UNIX_ALG_1       // ~298us и ~138B Flash (AVR)
#define UNIX_ALG UNIX_ALG_2       // ~216us и ~584B Flash (AVR)
#define UNIX_ALG UNIX_ALG_3       // ~297us и ~178B Flash (AVR)
#define UNIX_ALG UNIX_ALG_TIME_T  // ~246us и ~842B Flash (AVR)
```

> Все добавления времени в Datime выполняются напрямую, без конвертации в unix, т.е. довольно быстро.

### Stamp
Хранит дату и время в одной переменной `uint32_t` - UNIX время

```cpp
// ============ КОНСТРУКТОР ============
Stamp;
Stamp(uint32_t unix);           // из unix
Stamp(Datime& dt);              // из Datime
Stamp(year, month, day);        // год, месяц, день
Stamp(hour, minute, second);    // час, минута, секунда
Stamp(const char* str);         // из строки вида yyyy-mm-dd или hh:mm:ss или yyyy-mm-ddThh:mm:ss
Stamp(year, month, day, hour, minute, second);

// ============ УСТАНОВКА ============
void set(Datime& dt);           // установить из Datime
void set(year, month, day);     // год, месяц, день
void set(hour, minute, second); // час, минута, секунда
void set(year, month, day, hour, minute, second);

bool parse(const char* s);      // из строки вида yyyy-mm-dd или hh:mm:ss или yyyy-mm-ddThh:mm:ss
bool parseHTTP(const char* s);  // из строки вида <day_week>, dd <month> yyyy hh:mm:ss

// ============== ЭКСПОРТ =============
Datime get();           // экспортировать в формат Datime
void get(Datime& dt);   // экспортировать в переменную типа Datime
uint32_t toSeconds();   // получить секунды
uint32_t toMinutes();   // получить минуты
uint32_t toHours();     // получить часы
uint32_t toDays();      // получить сутки

// ============= В СТРОКУ =============
// вывести дату в формате "dd.mm.yyyy"
char* dateToChar(char* buf);
String dateToString();

// вывести время в формате "hh:mm:ss"
char* timeToChar(char* buf);
String timeToString();

// вывести в формате dd.mm.yyyy hh:mm:ss, div - разделитель
char* toChar(char* buf, char div = ' ');
String toString(char div = ' ');
```

### StampTicker
Тот же `Stamp`, но сохраняет счёт времени на базе `millis()` после синхронизации. Сигналит раз в секунду, можно подключить обработчик. Имеет тикер, нужно вызывать его в `loop()`.

```cpp
// установить текущий unix, дополнительно миллисекунды синхронизации
StampTicker(uint32_t unix, uint16_t ms = 0);

// установить текущий unix, дополнительно миллисекунды синхронизации
void update(uint32_t unix, uint16_t ms = 0);

void attach(f);             // подключить функцию-обработчик срабатывания (вида void f())
void detach();              // отключить функцию-обработчик срабатывания

bool ready();               // возвращает true каждую секунду
bool synced();              // время синхронизировано
uint16_t ms();              // получить миллисекунды текущей секунды
uint32_t calcUnix();        // вычислить и получить текущий unix

// тикер, вызывать в loop. Обновляет unix раз в секунду. Вернёт true каждую секунду с учётом мс синхронизации
bool tick();
```

### StampSync
Тот же `Stamp`, но сохраняет счёт времени на базе `millis()` после синхронизации. Работает без тикера.

```cpp
// установить текущий unix, дополнительно миллисекунды синхронизации
StampSync(uint32_t unix, uint16_t ms = 0);

// установить текущий unix, дополнительно миллисекунды синхронизации
void update(uint32_t unix, uint16_t ms = 0);

bool synced();      // время синхронизировано
uint32_t getUnix(); // получить текущий unix
uint16_t ms();      // получить миллисекунды текущей секунды
```

### Утилиты
Набор функций для работы со временем и датой

```cpp
// время в секунды
uint32_t StampUtils::timeToSeconds(uint8_t hours, uint8_t minutes, uint8_t seconds);

// високосный год
bool StampUtils::isLeap(uint16_t year);

// дней в месяце без учёта года (февраль 28)
uint8_t StampUtils::daysInMonth(uint8_t month);

// дней в месяце с учётом високосного года
uint8_t StampUtils::daysInMonth(uint8_t month, uint16_t year);

// дней года к месяцу (янв 0, фев 31, март 59/60...)
uint16_t StampUtils::daysToMonth(uint8_t month, uint16_t year);

// дата в день текущего года (начиная с 1)
uint16_t StampUtils::dateToYearDay(uint8_t day, uint8_t month, uint16_t year);

// дата в день недели (пн 1.. вс 7)
uint8_t StampUtils::dateToWeekDay(uint8_t day, uint8_t month, uint16_t year);

// дата в количество дней с 01.01.2000 (начиная с 0)
uint16_t StampUtils::dateToDays2000(uint8_t day, uint8_t month, uint16_t year);

// дата в unix время, zone в минутах
uint32_t StampUtils::dateToUnix(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t seconds, int16_t zone = 0);

```

<a id="example"></a>

## Пример
```cpp
Stamp s(2023, 5, 8, 16, 26, 0);
Serial.println(s.getUnix());
Serial.println(s.toString());

Datime d = s.get();
Serial.println(d.year);
Serial.println(d.month);
Serial.println(d.day);

d.year = 2022;  // изменили год
s.set(d);       // обновили Stamp
Serial.println(s.toString());
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - добавлено много новых инструментов и возможностей

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **Stamp** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/Stamp/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код