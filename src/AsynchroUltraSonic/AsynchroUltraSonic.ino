
constexpr int SonicSpeed = 340;

class HC_SR04_Asynchro{
  volatile unsigned long timeStamp;
  volatile unsigned long pulseLength;
  volatile bool isReading;
  const int sendTimeSpan/*ms*/, sendTimeLong/*us*/;
  const int readTimeMin/*us*/, readTimeMax/*us*/;
  int distance;
  uint8_t pineEcho, pinTrig;
 public:
  HC_SR04_Asynchro(
      const int pin_trig = -1, const int pin_echo = -1,
      const int send_time_span = 200, const int send_time_long = 10,
      const int read_time_min = 100, const int read_time_max = ):
    pinTrig(pin_trig), pinEcho(pin_echo),
    sendTimeSpan(send_time_span), sendTimeLong(send_time_long),
    readTimeMin(read_time_min), readTimeMax(read_time_max)
  {};
  
  bool begin(const int Trig = -1, const int Echo = -1){
    if(this->pinTrig<0) this->pinTrig = Trig;
    if(this->pinEcho<0) this->pinEcho = Echo;
    if(this->pineEcho<0 || this->pineTrig<0) return false;
    pinMode(this->pinTrig, OUTPUT);
    pinMode(this->pineEcho, INPUT);
    attachInterruptArg(digitalPinToInterrupt(this->pinEcho), echoISR, this, CHNGE);
    return true;
  }

  bool send(){
    if(this->isReading || this->timeStamp <= this->sendTimeSpan) return false;
    //actual send procedure
    return true;
  }

  int read(){}

  static void IRAM_ATTR echoISR(void* arg){
    HC_SR04_Asynchro* self = static_cast<HC_SR04_Asynchro*>(arg);
    if(digitalRead(self->pineEcho)){//when pulse started
      if(self->isReading) return;//さすがにここは動かないでほしい
      self->timeStamp = micros();
      self->isReading = true;
    }else{//when pulse ended
      if(!self->isReading) return;//さすがにここは動かないでほしい
      self->pulseLength = micros()-self->timeStamp;
      self->timeStamp += self->pulseLength;
      self->isReading = false;
    }
  }

};

void setup(){}
void loop(){}