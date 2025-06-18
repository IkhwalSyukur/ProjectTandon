#pragma once
#include <Arduino.h>
#include <Unit_Test/WaterLevel.h>

// JSN_GPIO jsnSensor(33, 32); // JSN-SR04T sensor on pins 33 (trig) and 32 (echo)
JSN_GPIO jsnSensor(18, 19); // JSN-SR04T sensor on pins 33 (trig) and 32 (echo)


class RelayTest
{
    private:
        uint8_t pin;
        bool status;
    public:
        RelayTest(uint8_t pin) : pin(pin) {}
        void begin() {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW); // Ensure relay is off initially
        }

        void setRelay(bool state) {
            digitalWrite(pin, state ? HIGH : LOW);
        }

        bool getRelayState() {
            return digitalRead(pin);
        }
        void toggleRelay() {
            bool currentState = getRelayState();
            setRelay(!currentState);
        }
        void testRelay() {
            Serial.println("Testing Relay...");
            setRelay(true);
            Serial.println("Relay ON");
            delay(5000); // Wait for 1 second
            setRelay(false);
            Serial.println("Relay OFF");
        } 
};


RelayTest relayTest(25); // Pin 5 is used for the relay

bool relayStatus = true; // Variable to track relay status

void  setup() {
    Serial.begin(115200);
    relayTest.begin();
    jsnSensor.begin(); // Initialize the JSN-SR04T sensor
}

void loop() {
    // relayTest.testRelay(); // Test the relay functionality
    // delay(5000); // Wait for 2 seconds before the next test
    // Serial.println(relayStatus ? "Relay is ON" : "Relay is OFF");
    int jsn = jsnSensor.getDistance();
    if ( jsn >= 59 && relayStatus == true) {
        relayTest.setRelay(false);
        Serial.println("air habis, relay ON");
        relayStatus = false; // Update the relay status
    } 
    else if (jsn <= 25 && relayStatus == false) {
        relayTest.setRelay(true);
        Serial.println("air penuh, relay OFF");
        relayStatus = true; // Update the relay status
    }

    else{
        relayTest.setRelay(relayStatus);
    }
        Serial.print("Distance: ");
        Serial.println(jsn);
    delay(2000); // Wait for 2 seconds before the next test
}