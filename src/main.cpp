// #define UNIT_TEST // Comment this line to run the driver code
#include <Arduino.h>
#include "esp_log.h"

#ifndef UNIT_TEST // If UNIT_TEST is not defined, include the driver code
#include <WaterTorrentManager.h>
#include <driver/WaterLevel_JSN04T.h>
#include <driver/WaterServo.h>
#include <driver/WaterFlow_YFBX.h>
#include <driver/WaterPump.h>
#include <driver/WaterTimer_RTC.h>
#include <driver/Water_Fuzzy.h>

// Driver setup ESP A
WaterLevel_JSN04T waterLevel(33, 32); // Trig pin, Echo pin
WaterServo waterServo(14);           // Servo pin
WaterFlow_YFBX waterFlow;
WaterPump waterPump(25); // Pump pin              
WaterTimer_RTC timerWater; // RTC object
WaterFuzzy waterfuzzy;

// Driver setup ESP B
// WaterLevel_JSN04T waterLevel(18, 19); // Trig pin, Echo pin
// WaterServo waterServo(26);           // Servo pin
// WaterFlow_YFBX waterFlow;
// WaterPump waterPump(23); // Pump pin              
// WaterTimer_RTC timerWater; // RTC object
// WaterFuzzy waterfuzzy;

WaterTorrentManager waterTorrent(waterLevel, waterServo, waterFlow, waterPump, timerWater,waterfuzzy); // WaterTorrentManager object

void IRAM_ATTR pulseCounter()
{
    waterFlow.incrementPulse(); // Increment pulse count in the flow sensor
}

void setup()
{
    Serial.begin(115200);
    waterTorrent.begin();

    attachInterrupt(digitalPinToInterrupt(27), pulseCounter, FALLING); // Flow interupt

    void monitoringTask(void *param);
    void fuzzyTask(void *param);
    void RelayTask(void *param);

    
    
    xTaskCreate(monitoringTask, "MonitoringTask", 2048, NULL, 1, NULL);
    xTaskCreate(fuzzyTask, "FuzzyTask", 2048, NULL, 1, NULL); 
    xTaskCreate(RelayTask, "RelayTask", 2048, NULL, 1, NULL);


}

void loop()
{
    
}

void fuzzyTask(void *param)
{
    while (true)
    {
        waterTorrent.runFuzzy(); // Run fuzzy logic control
        delay(1000); // Delay for 1 second
    }
}

void RelayTask(void *param)
{
    while (true)
    {
        waterTorrent.RelayDriver(); // Control pump based on water level
        delay(1000); // Delay for 1 second
    }
}

void monitoringTask(void *param)
{
    while (true)
    {
        waterTorrent.readingWaterLevel(); // Read water level
        waterTorrent.readingFlowRate();   // Read flow rate

        

        // Print data to Serial Monitor
        Serial.printf("Water Level: %.2f cm\n", waterTorrent.getWaterLevel());
        Serial.printf("Flow Rate: %.2f L/min\n", waterTorrent.getFlowRate());
        Serial.printf("Liters per Day: %d L\n", waterTorrent.getLitersPerDay());
        Serial.printf("Fuzzy Output: %.2f\n", waterTorrent.runFuzzy());

        // waterTorrent.setAngle(90); // Set servo angle to 90 degrees

        delay(1000); // Delay for 1 second
    }
}


#else // If UNIT_TEST is defined, include the unit test code
// #include <Unit_Test/Flow_Test.h>
// #include <Unit_Test/WaterLevel.h>
// #include <Unit_Test/Fuzzy_Test.h>
// #include <Unit_Test/Servo_Pump.h>
#include <Unit_Test/relay_Test.h>


// FlowTest flowTest; // Create an instance of FlowTest


// void IRAM_ATTR pulseCounter()
// {
//   flowTest.incrementPulse(); 
// }

// void setup()
// {
//     Serial.begin(115200);
//     flowTest.begin();
//     attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
// }

// void loop()
// {

//     float thisflow = flowTest.getFlow(); 
//     Serial.printf("Flow rate: %.2f L/min\n", thisflow);
// }

#endif