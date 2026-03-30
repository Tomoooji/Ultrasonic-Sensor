#include "UltraSonic.h"
HC_SR04 test;
int dist;

void setup(){
  Serial.begin(9600);
  test.attach(/*Trig=*/19, /*Echo=*/21);
}

void loop(){
  Serial.println(test.readDist()<200? test.distance: -1);
  delay(10);
}
