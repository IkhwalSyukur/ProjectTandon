#pragma once
#include <interfaces/WaterPumpInterface.h>
#include <Arduino.h>


class WaterPump : public WaterPumpInterface
{
private:
    uint8_t pumpPin; // Pin for the water pump
    bool pumpStatus; // Status of the water pump (on/off)

public:
    WaterPump(uint8_t pin) : pumpPin(pin), pumpStatus(false) {} // Constructor with default pin 25
    bool begin();
    void setPump(bool status);
    void testpump();
    bool getPumpStatus();
};

