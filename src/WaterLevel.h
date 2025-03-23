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

class JSN_UART
{
private:
    HardwareSerial &uart;
    uint8_t rxpin;
    uint8_t txpin;

public:
    JSN_UART(HardwareSerial &uartPort, uint8_t rx, uint8_t tx) : uart(uartPort) , rxpin(rx), txpin(tx) {}

    void begin(uint32_t baud = 9600) {
        uart.begin(baud, SERIAL_8N1, rxpin, txpin);
    }

    float getdistance(){
        if (uart.available() >= 4) {
            uint8_t buffer[4];
            uart.readBytes(buffer, 4);
    
            if (buffer[0] == 0xFF) {
              uint16_t distance_mm = (buffer[1] << 8) + buffer[2];
              return distance_mm / 10.0;
            }
          }
          return -1.0; // Invalid or no data
        }
};


