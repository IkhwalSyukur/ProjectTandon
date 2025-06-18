#pragma once

class WaterPumpInterface
{
public:
    virtual bool begin() = 0;
    virtual void setPump(bool status);
    virtual void testpump();
};
