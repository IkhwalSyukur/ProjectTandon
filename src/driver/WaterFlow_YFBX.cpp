#include <driver/WaterFlow_YFBX.h>
#include <Arduino.h>

#define LED_BUILTIN 2
#define SENSOR  18

void IRAM_ATTR WaterFlow_YFBX::incrementPulse(){
    pulseCount++;
}

bool WaterFlow_YFBX::begin(){

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SENSOR, INPUT_PULLUP);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;

    return true;

}

uint32_t WaterFlow_YFBX::getPulseCount() {
    return pulseCount;
}

float WaterFlow_YFBX::getFlowRate(){
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
        pulse1Sec = pulseCount;
        pulseCount = 0;

        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = currentMillis;

        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;
    }

    return flowMilliLitres;
}

int WaterFlow_YFBX::getLiters() {
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
        pulse1Sec = pulseCount;
        pulseCount = 0;

        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = currentMillis;

        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;
    }

    return totalMilliLitres / 1000;
}