[![latest](https://img.shields.io/github/v/release/GyverLibs/Stamp.svg?color=brightgreen)](https://github.com/GyverLibs/Stamp/releases/latest/download/Stamp.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/Stamp?_x_tr_sl=ru&_x_tr_tl=en)

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# Stamp
> Бета-версия!

Библиотека для хранения и манипуляции со временем
- Более удобная в использовании, чем встроенная time.h
- Используются более быстрые алгоритмы преобразования, чем в time.h
- Хранит дату и время в 4 байт переменной (секунды с 01.01.2000)
- Преобразует в год, месяц, день, часы, минуты, секунды, день недели, день года

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Документация](#reference)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

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

<a id="reference"></a>
## Документация
### TimeZone
```cpp
void setStampZone(int zone);    // установить глобальную часовую зону в часах или минутах
int getStampZone();             // получить глобальную часовую зону
```

### Datime
Хранит дату и время в отдельных переменных:
- `year`/`y` - год
- `month`/`mon`/`mo` - месяц (1.. 12)
- `day`/`mday`/`d` - день (1.. 28-31)
- `hour`/`h` - час (0.. 23)
- `minute`/`min`/`m` - минута (0.. 59)
- `second`/`sec`/`s` - секунда (0.. 59)
- `weekDay`/`wday`/`wd` - день недели (1 пн.. 7 вс)
- `yearDay`/`yday`/`yd` - день года (1.. 365-366)

```cpp
// ============ КОНСТРУКТОР ============
Datime;
Datime(year, month, day);           // год, месяц, день
Datime(hour, minute, second);       // час, минута, секунда
Datime(year, month, day, hour, minute, second);

// ============= УСТАНОВКА =============
void set(year, month, day);         // год, месяц, день
void set(hour, minute, second);     // час, минута, секунда
void set(year, month, day, hour, minute, second);

// ============== ЭКСПОРТ ==============
uint32_t toSeconds();               // вывести в секунды

// ============= В СТРОКУ ==============
// вывести дату в формате "dd.mm.yyyy"
char* dateToChar(char* buf);
String dateToString();

// вывести время в формате "hh:mm:ss"
char* timeToChar(char* buf);
String timeToString();

// вывести в формате dd.mm.yyyy hh:mm:ss
char* toChar(char* buf);
String toString();
```

### Stamp
Хранит дату и время в одной переменной `uint32_t` - UNIX время

```cpp
// ============ КОНСТРУКТОР ============
Stamp;
Stamp(uint32_t u);              // из uint32_t
Stamp(Datime& dt);              // из Datime
Stamp(year, month, day);        // год, месяц, день
Stamp(hour, minute, second);    // час, минута, секунда
Stamp(year, month, day, hour, minute, second);

// ============ УСТАНОВКА ============
void set(Datime& dt);               // установить из Datime
void set(year, month, day);         // год, месяц, день
void set(hour, minute, second);     // час, минута, секунда
void set(year, month, day, hour, minute, second);

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

// вывести в формате dd.mm.yyyy hh:mm:ss
char* toChar(char* buf);
String toString();
```

### StampTicker
Тот же `Stamp`, но поддерживает время через `millis()` с возможностью синхронизации

```cpp
void update(uint32_t dt);   // установить текущий unix
bool synced();              // время синхронизировано
```

<a id="example"></a>
## Пример
```cpp
Stamp s(2023, 5, 8, 16, 26, 0);
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