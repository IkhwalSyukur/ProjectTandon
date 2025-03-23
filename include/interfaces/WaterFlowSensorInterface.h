#pragma once

class WaterFlowSensorInterface
{
    public:
        virtual bool begin() = 0;
        virtual float getFlowRate() = 0;
        virtual float getVolume() = 0;
        virtual float getVolumePerHour() = 0;
        virtual float getVolumePerDay() = 0;
};