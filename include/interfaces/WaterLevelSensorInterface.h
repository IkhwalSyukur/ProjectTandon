#pragma once

class WaterLevelSensorInterface
{
    public:
        virtual bool begin() = 0;
        virtual float getDistance() = 0;
};