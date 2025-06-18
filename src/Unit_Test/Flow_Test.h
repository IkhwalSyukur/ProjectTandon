#pragma once
#include <Arduino.h>
// #define LED_BUILTIN 2
#define SENSOR  27


class FlowTest
{
private:
    long currentMillis = 0;
    long previousMillis = 0;
    int interval = 1000;
    boolean ledState = LOW;
    // float calibrationFactor = 10; //tandon B
    float calibrationFactor = 8; //tandon A
    volatile uint32_t pulseCount;
    byte pulse1Sec = 0;
    float flowRate;
    unsigned int flowMilliLitres;
    unsigned long totalMilliLitres;

public:

    FlowTest() : pulseCount(0), flowRate(0.0), flowMilliLitres(0), totalMilliLitres(0) {}

    void IRAM_ATTR incrementPulse(){
        pulseCount++;
    }

    void begin(){
        // pinMode(LED_BUILTIN, OUTPUT);
        pinMode(SENSOR, INPUT_PULLUP);

        pulseCount = 0;
        flowRate = 0.0;
        flowMilliLitres = 0;
        totalMilliLitres = 0;
        previousMillis = 0;

    }

    uint32_t getPulseCount() {
        return pulseCount;
    }

    float getFlow(){
        currentMillis = millis();
        if (currentMillis - previousMillis > interval) {
            pulse1Sec = pulseCount;
            pulseCount = 0;

            flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
            previousMillis = currentMillis;

            flowMilliLitres = (flowRate / 60) * 1000;
            totalMilliLitres += flowMilliLitres;
        }

        return totalMilliLitres / 1000.0; // Return total flow in liters
    }


};


