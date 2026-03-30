#pragma once
#if defined(ESP32)
#include <Arduino.h>

constexpr int SonicSpeed = 340;

class HC_SR04_Asynchro{
  volatile unsigned long sendTimeStamp;
  volatile unsigned long readTimeStamp;
  volatile unsigned long pulseLength;
  volatile bool isReading;
  const int sendTimeSpan/*ms*/, sendTimeLong/*us*/;
  const int readTimeMin/*us*/, readTimeMax/*us*/;
  int distance;
  uint8_t pinEcho, pinTrig;
 public:
  HC_SR04_Asynchro(
      const int send_time_span = 200, const int send_time_long = 10,
       //↑最短だと60だが多めに見積もって200とする資料も多い
      const int read_time_min = 100, const int read_time_max = 23500):
    sendTimeSpan(send_time_span), sendTimeLong(send_time_long),
    readTimeMin(read_time_min), readTimeMax(read_time_max){
  };
  
  void begin(uint8_t trig, uint8_t echo){
    this->pinTrig = trig; this->pinEcho = echo;
    pinMode(this->pinTrig, OUTPUT);
    pinMode(this->pinEcho, INPUT);
    attachInterruptArg(digitalPinToInterrupt(this->pinEcho), echoISR, this, CHANGE);
    this->sendTimeStamp = millis();
  }

  bool send(){
    if(millis() - this->sendTimeStamp <= this->sendTimeSpan) return false;
    //actual send procedure
    digitalWrite(this->pinTrig, HIGH);
    delayMicroseconds(this->sendTimeLong);
    digitalWrite(this->pinTrig, LOW);
    this->sendTimeStamp = millis();
    return true;
  }

  int read(){
    return SonicSpeed/2*this->pulseLength*0.0001;
  }

  static void IRAM_ATTR echoISR(void* arg){
    HC_SR04_Asynchro* self = static_cast<HC_SR04_Asynchro*>(arg);
    if(digitalRead(self->pinEcho)){//when pulse started
      if(self->isReading) return;//さすがにここは動かないでほしい
      self->readTimeStamp = micros();
      self->isReading = true;
    }else{//when pulse ended
      if(!self->isReading) return;//さすがにここは動かないでほしい
      self->pulseLength = micros()-self->readTimeStamp;
      self->readTimeStamp += self->pulseLength;
      self->isReading = false;
    }
  }

};

#endif