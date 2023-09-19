#include <Arduino.h>
#include <StampTicker.h>

StampTicker sync;

void setup() {
    Serial.begin(115200);
    // sync.update(Stamp("2022-12-05T12:25:34"));
    sync.update(1695146928);
}
void loop() {
  Serial.println(sync.toString());
  delay(1000);
}