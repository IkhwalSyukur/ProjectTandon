#pragma once

#include <Arduino.h>
#include <Esp32Servo.h>



class Servo_Pump
{
private:
    uint8_t pin;
    Servo servo;
    int currentAngle = 0;       // Current servo angle
    int targetAngle = 0;        // Desired target angle
    int stepSize = 5;           // How many degrees per step
    unsigned long lastMoveTime = 0;
    unsigned long stepDelay = 15;

public:
    Servo_Pump(uint8_t pin) : pin(pin) {}
    void begin() {
        pinMode(pin, OUTPUT);
        servo.attach(pin);
        servo.write(0); // Initialize servo to 0 degrees
    }

    void degree(int degValue) {
        static unsigned long lastCommandTime = 0;
        if (millis() - lastCommandTime > 500) {
            lastCommandTime = millis();
            targetAngle = degValue;
            update(); // Call update to move the servo
        } 

    }

    void update() {
        unsigned long currentTime = millis();
        if (currentTime - lastMoveTime >= stepDelay) {
            if (currentAngle < targetAngle) {
                currentAngle += stepSize;
                if (currentAngle > targetAngle) {
                    currentAngle = targetAngle; // Ensure we don't overshoot
                }
            } else if (currentAngle > targetAngle) {
                currentAngle -= stepSize;
                if (currentAngle < targetAngle) {
                    currentAngle = targetAngle; // Ensure we don't undershoot
                }
            }
            servo.write(currentAngle);
            lastMoveTime = currentTime;
        }
    }
};
Servo_Pump servo_pump(14); // Pin 13 is used for the servo motor

void setup()
{
    Serial.begin(115200);
    servo_pump.begin();
    // servo_pump.degree(0);

}

void loop()
{

}
//     for (int i = 0; i <= 180; i += 10) {
//         servo_pump.degree(i);
//         Serial.printf("Servo angle: %d\n", i);
//         delay(2000); // Wait for 500 milliseconds
//     }

//     // servo_pump.degree(0); // Set servo to 90 degrees
//     // Serial.println("Servo set to 0 degrees");
//     // delay(3000); // Wait for 2 seconds
//     servo_pump.degree(0); // Set servo to 180 degrees
//     Serial.println("Servo set to 180 degrees");
//     delay(3000); // Wait for 2 seconds  
// }