#include "WaterPump.h"

bool WaterPump::begin()
{
    pinMode(pumpPin, OUTPUT); // Set the pump pin as output
    digitalWrite(pumpPin, LOW); // Initialize the pump to off (LOW)
    return true;
}

void WaterPump::setPump(bool state)
{
    digitalWrite(pumpPin, state ? HIGH : LOW);
}


void WaterPump::testpump()
{
    Serial.println("Testing Water Pump...");
    setPump(true); // Turn the pump on
    Serial.println("Pump ON");
    delay(2000); // Wait for 1 second
    setPump(false); // Turn the pump off
    Serial.println("Pump OFF");
    delay(2000); // Wait for 1 second before next test
}

bool WaterPump::getPumpStatus()
{
    return digitalRead(pumpPin); // Return true if the pump is on
}
