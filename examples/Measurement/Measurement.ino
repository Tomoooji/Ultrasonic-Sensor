/* Measurement.ino
  print distance with Serial by Arduino UNO / ESP32
  measurement is asynchronous on ESP32
*/

#include <UltraSonic.h>
#include <UltraSonicAsync.h>

const uint8_t pin_trig = 4;
const uint8_t pin_echo = 2;

#if defined(ARDUINO_ARCH_ESP32)
HCSR04Async sensor(pin_trig, pin_echo, 
                    10 /*microsec*/,
                    200 /*millisec*/,
                    1000 /*millisec*/);
#else
HCSR04 sensor(pin_trig, pin_echo, 
              10 /*microsec*/,
              200 /*millisec*/,
              1000 /*millisec*/);
#endif

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
#if defined(ARDUINO_ARCH_ESP32)
  sensor.read();
  float distance = sensor.getDistance();
#else
  float distance = sensor.read();
#endif
  Serial.println(distance);
  delay(200);
}