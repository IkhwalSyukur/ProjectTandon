#include "WaterTimer_RTC.h"


bool WaterTimer_RTC::beginTimer()
{
    Wire.begin(); // Initialize I2C communication
    rtc.begin(); // Initialize the RTC

    if (!rtc.begin()){
        Serial.println("Couldn't find RTC"); // Check if RTC is connected
        return false; // Return false if RTC is not found
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting the time!"); // Check if RTC lost power
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set the RTC to the current date and time
        
        // rtc.adjust(DateTime(2023, 10, 1, 0, 0, 0)); // Set the RTC to a specific date and time (optional)
    }

    return true; // Return a boolean value as required
}

int WaterTimer_RTC::getHour()
{
    DateTime now = rtc.now(); // Get the current date and time from the RTC
    return now.hour(); // Return the current hour
}

int WaterTimer_RTC::getDay()
{
    DateTime now = rtc.now(); // Get the current date and time from the RTC
    return now.day(); // Return the current day of the month
}