#include "WaterLevel_JSN04T.h"

bool WaterLevel_JSN04T::begin()
{
    pinMode(trigpin, OUTPUT);
    pinMode(echopin, INPUT);
    return true;
}

float WaterLevel_JSN04T::getDistance()
{
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);

    long duration = pulseIn(echopin, HIGH, 30000); // 30ms timeout
    if (duration == 0)
    {
        return -1.0; // Timeout or no detection
    }

    return (duration / 2.0) / 29.1;
}