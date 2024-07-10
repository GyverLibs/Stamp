#include <Arduino.h>
#include <StampTicker.h>

StampTicker st;

void setup() {
    Serial.begin(115200);
    // st.update(Stamp("2022-12-05T12:25:34"));
    st.update(1695146928);
}
void loop() {
  Serial.println(st.toString());
  delay(1000);
}