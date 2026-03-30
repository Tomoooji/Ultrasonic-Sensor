
constexpr int SonicSpeed = 340;

class HC_SR04_Asynchro{
  volatile unsigned long timeStamp;
  volatile unsigned long pulseLength;
  volatile bool isReading;
  volatile bool isWaiting;
  const int sendTimeSpan/*ms*/, sendTimeLong/*us*/;
  const int readTimeMin/*us*/, readTimeMax/*us*/;
  int distance;
  uint8_t pineEcho, pinTrig;
 public:
  HC_SR04_Asynchro(
      const int send_time_span = 200, const int send_time_long = 10,
      //⇧最短だと60だが多めに見積もって200とする資料も多い
      const int read_time_min = 100, const int read_time_max = 23500):
    sendTimeSpan(send_time_span), sendTimeLong(send_time_long),
    readTimeMin(read_time_min), readTimeMax(read_time_max){
  };
  
  void begin(uint8_t trig, uint8_t echo){
    this->pinTrig = trig; this->pinEcho = echo;
    pinMode(this->pinTrig, OUTPUT);
    pinMode(this->pineEcho, INPUT);
    attachInterruptArg(digitalPinToInterrupt(this->pinEcho), echoISR, this, CHNGE);
  }

  bool send(){
    if(this->isReading || this->timeStamp <= this->sendTimeSpan) return false;
    //actual send procedure
    digitalWrite(this->pinTrig, HIGH);
    delayMicrosecond(this->sendTimeLong);
    digitalWrite(this->pinTrig, LOW);
    this->isWaiting = true;
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