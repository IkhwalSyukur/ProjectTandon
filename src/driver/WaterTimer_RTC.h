#pragma once
#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>
#include <interfaces/WaterTimeInterface.h>


class WaterTimer_RTC : public WaterTimeInterface
{
private:
    RTC_DS3231 rtc; // RTC object

public:
    bool beginTimer(); // Initialize the RTC
    int getHour(); // Get the current hour
    int getDay(); // Get the current day of the month
};
