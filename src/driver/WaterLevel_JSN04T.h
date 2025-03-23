#pragma once
#include <interfaces/WaterLevelSensorInterface.h>

#include <Arduino.h>

class WaterLevel_JSN04T : public WaterLevelSensorInterface
{
    public:
        WaterLevel_JSN04T(uint8_t trig, uint8_t echo) : trigpin(trig), echopin(echo) {}
        bool begin();
        float getDistance();

    private:
        uint8_t trigpin;
        uint8_t echopin;
};