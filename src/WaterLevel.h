#pragma once
#include <Arduino.h>

class JSN_GPIO
{
private:
    uint8_t trigpin;
    uint8_t echopin;

public:
    JSN_GPIO(uint8_t trig, uint8_t echo) : trigpin(trig), echopin(echo) {}
    void begin() {
        pinMode(trigpin, OUTPUT);
        pinMode(echopin, INPUT);
    }

    float getDistance(){
        digitalWrite(trigpin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigpin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigpin, LOW);
  
        long duration = pulseIn(echopin, HIGH, 30000); // 30ms timeout
        if(duration == 0) {
          return -1.0; // Timeout or no detection
        }
  
        return (duration / 2.0) / 29.1;
      }
};

class JSN_GPIO2
{
private:
    uint8_t trigpin2;
    uint8_t echopin2;

public:
    JSN_GPIO2(uint8_t trig2, uint8_t echo2) : trigpin2(trig2), echopin2(echo2) {}
    void begin() {
        pinMode(trigpin2, OUTPUT);
        pinMode(echopin2, INPUT);
    }

    float getDistance(){
        digitalWrite(trigpin2, LOW);
        delayMicroseconds(2);
        digitalWrite(trigpin2, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigpin2, LOW);
  
        long duration = pulseIn(echopin2, HIGH, 30000); // 30ms timeout
        if(duration == 0) {
          return -1.0; // Timeout or no detection
        }
  
        return (duration / 2.0) / 29.1;
      }
};