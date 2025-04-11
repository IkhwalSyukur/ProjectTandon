#pragma once
#include <Arduino.h>
#include <interfaces/WaterFlowSensorInterface.h>


class WaterFlow_YFBX : public WaterFlowSensorInterface
{
    private:
        long currentMillis = 0;
        long previousMillis = 0;
        int interval = 1000;
        boolean ledState = LOW;
        float calibrationFactor = 4.5;
        volatile uint32_t pulseCount;
        byte pulse1Sec = 0;
        float flowRate;
        unsigned int flowMilliLitres;
        unsigned long totalMilliLitres;
    
    public:
        WaterFlow_YFBX() : pulseCount(0), flowRate(0.0), flowMilliLitres(0), totalMilliLitres(0) {}
        void IRAM_ATTR incrementPulse();
        bool begin();
        uint32_t getPulseCount();
        float getFlowRate();
};