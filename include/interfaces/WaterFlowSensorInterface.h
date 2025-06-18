#pragma once

class WaterFlowSensorInterface
{
    public:
        virtual bool begin() = 0;
        virtual float getFlowRate() = 0;
        virtual int getLiters() = 0; 
};