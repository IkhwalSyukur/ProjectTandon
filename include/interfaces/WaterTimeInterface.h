#pragma once

class WaterTimeInterface
{
public:
    virtual bool beginTimer() = 0;
    virtual int getHour() = 0;
    virtual int getDay() = 0;
};