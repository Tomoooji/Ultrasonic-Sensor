#pragma once
#include <Arduino.h>

constexpr int SonicSpeed = 340;

class HC_SR04{
 public:
  uint8_t pin_trig;
  uint8_t pin_echo;
  const int send_time_long;//[microsec]
  const int wait_time_long;//[millisec]
  const unsigned long timeout;//[microsec]
  unsigned long last_send_time = -1;
  float distance;

  HC_SR04(const unsigned long timeout = 100000/*microsec*/, const int SendTimeLong = 10/*microsec*/, const int WaitTimeLong = 200/*millisec*/):
    send_time_long(SendTimeLong), wait_time_long(WaitTimeLong), timeout(timeout){}

  void attach(uint8_t pinTrig, uint8_t pinEcho){
    this->pin_trig = pinTrig;
    this->pin_echo = pinEcho;
    pinMode(this->pin_trig, OUTPUT);
    pinMode(this->pin_echo, INPUT);
  }

  int readDist(){
    if(this->last_send_time==-1 || millis()-this->last_send_time > this->wait_time_long){
      //digitalWrite(this->pin_trig, LOW);
      //delayMicroseconds(2);
      digitalWrite(this->pin_trig, HIGH);
      delayMicroseconds(send_time_long);
      digitalWrite(this->pin_trig, LOW);
      return this->distance = this->calcDist(pulseIn(this->pin_echo, HIGH, this->timeout));
    }
    return -1;
  }

  int calcDist(unsigned long pulseWidth){
    return SonicSpeed/2*pulseWidth*0.0001;
  }

};