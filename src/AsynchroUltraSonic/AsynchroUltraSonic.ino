#include "AsynchroUltraSonic.h"

HC_SR04_Asynchro Sensor;

void setup(){
  Serial.begin(115200);
  Sensor.begin(16,17);
}
void loop(){
  delay(1);
  Sensor.send();
  Serial.println(Sensor.read());
}