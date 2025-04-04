#pragma once

class WaterServoInterface
{
public:    
    
    virtual bool begin() = 0;
    virtual void setAngle(int angle) = 0;
};

