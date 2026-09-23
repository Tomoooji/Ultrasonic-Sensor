#pragma onece
#include <Arduino.h>

class HC_SR04_Async{
 private:
 uint8_t pin_trig, pin_echo;
 const int us_send_time_long;//[microsec]
 const int ms_wait_time_long;//[millisec]
 const unsigned long ms_timeout;//[microsec]

 float distance;

 const uint8_t instance_id;                                                                                                                                                                                                 
 static uint8_t instance_count;
 volatile static uint8_t current_read_instance;
 
  inline static HC_SR04_Async* _instance = nullptr;

  static void read_dist_isr(){

  }

 public:
  HC_SR04(const unsigned long timeout = 100000/*microsec*/, const int SendTimeLong = 10/*microsec*/, const int WaitTimeLong = 200/*millisec*/):
    us_send_time_long(SendTimeLong), ms_wait_time_long(WaitTimeLong), ms_timeout(timeout),instance_id(instance_count){
        instance_count += 1;
    }

  void attach(uint8_t pinTrig, uint8_t pinEcho){
    this->pin_trig = pinTrig;
    this->pin_echo = pinEcho;
    pinMode(this->pin_trig, OUTPUT);
    pinMode(this->pin_echo, INPUT);
  }

  int calcDist(unsigned long pulseWidth){
    return SonicSpeed/2*pulseWidth*0.0001;
  }

};
