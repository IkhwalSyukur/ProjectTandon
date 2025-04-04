#pragma once
#include <Arduino.h>
#include <interfaces/WaterServoInterface.h>

class WaterServo : public WaterServoInterface
{
private:
    uint8_t Servopin;
    
public:
    WaterServo(int pin) : Servopin(pin) {}
    bool begin();
    void setAngle(int angle) override;
    
};
