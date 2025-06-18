#pragma once
#include <Arduino.h>
#include <interfaces/WaterServoInterface.h>

class WaterServo : public WaterServoInterface
{
private:
    uint8_t Servopin;
    int currentAngle = 0;       // Current servo angle
    int targetAngle = 0;        // Desired target angle
    int stepSize = 5;           // How many degrees per step
    unsigned long lastMoveTime = 0;
    unsigned long stepDelay = 15;
    
public:
    WaterServo(int pin) : Servopin(pin) {}
    bool begin();
    void setAngle(int angle) override;
    void update();
   
    
};
