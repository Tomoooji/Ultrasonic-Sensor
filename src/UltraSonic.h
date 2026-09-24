#pragma once
#include <Arduino.h>

constexpr int SONIC_SPEED = 340;

class HCSR04 {
protected:
  const uint8_t &pin_trig;
  const uint8_t &pin_echo;
  const int send_pulse_length; //[microsec]
  const int send_span_min;     //[millisec]
  const int read_timeout;      //[millisec]

public:
  HCSR04(const uint8_t &pin_trig, const uint8_t &pin_echo,
         const int send_pulse_length = 10 /*microsec*/,
         const int send_span_min = 200 /*millisec*/,
         const int read_timeout = 1000 /*millisec*/)
      : pin_trig(pin_trig), pin_echo(pin_echo), send_pulse_length(send_pulse_length),
        send_span_min(send_span_min), read_timeout(read_timeout) {};

        HCSR04(const uint8_t (&pins)[2],
         const int send_pulse_length = 10 /*microsec*/,
         const int send_span_min = 200 /*millisec*/,
         const int read_timeout = 1000 /*millisec*/)
      : HCSR04(pins[0], pins[1], send_pulse_length, send_span_min, read_timeout) {}

  virtual ~HCSR04() = default;

  virtual void begin() {
    pinMode(this->pin_trig, OUTPUT);
    digitalWrite(this->pin_trig, LOW);
    pinMode(this->pin_echo, INPUT);
  }

  float read() {
    digitalWrite(this->pin_trig, HIGH);
    delayMicroseconds(this->send_pulse_length);
    digitalWrite(this->pin_trig, LOW);
    unsigned long pulse = pulseIn(this->pin_echo, HIGH, this->read_timeout * 1000);
    return pulse > 0 ? this->calcDistance(pulse) : -1.0f;
  }

  static float calcDistance(unsigned long pulseWidth) {
    return SONIC_SPEED / 2 * pulseWidth * 0.0001;
  }
};
