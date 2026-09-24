#pragma once
#if defined(ARDUINO_ARCH_ESP32)

#include "UltraSonic.h"
#include <atomic>

class HCSR04Async : public HCSR04 {
private:
  TaskHandle_t taskHandle;
  std::atomic<float> distance{0.0f};

  static void task_starter(void *arg) {
    HCSR04Async *self = static_cast<HCSR04Async *>(arg);
    for (;;) {
      self->read();
      vTaskDelay(pdMS_TO_TICKS(self->send_span_min));
    }
  }

public:
  using HCSR04::HCSR04;
  HCSR04Async(HCSR04Async &&other) noexcept
      : HCSR04(std::move(other)), distance(other.distance.load()) {}
  /* ここら辺は一旦使わないだろうのでコメントアウト
  HCSR04Async(const HCSR04Async &other)
      : HCSR04(std::move(other)), distance(other.distance.load()) {}
  */

  void begin() override {
    HCSR04::begin();
    xTaskCreatePinnedToCore(task_starter, "HCSR04Async_Task", 2048, this, 1,
                            &taskHandle, 0);
  }

  void read() { this->distance.store(HCSR04::read()); }

  const float getDistance() const { return this->distance.load(); }
};

#endif