#include   "WaterServo.h"
#include <esp32Servo.h>



bool WaterServo::begin() 
{
    pinMode(Servopin, OUTPUT);
    return true;
}

void WaterServo::setAngle(int angle) 
{
    Servo servo;
    servo.attach(Servopin);
    servo.write(angle);
}