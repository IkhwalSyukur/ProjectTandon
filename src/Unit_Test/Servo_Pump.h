#pragma once

#include <Arduino.h>
#include <Esp32Servo.h>

class Servo_Pump
{
private:
    uint8_t pin;

public:
    Servo_Pump(uint8_t pin) : pin(pin) {}
    void begin() {
        pinMode(pin, OUTPUT);
    }

    void degree(int degValue) {
        Servo servo;
        servo.attach(pin);
        servo.write(degValue);
    }
};

Servo_Pump servo_pump(13); // Pin 13 is used for the servo motor

void setup()
{
    Serial.begin(115200);
    servo_pump.begin();
}

void loop()
{
    servo_pump.degree(0);
    delay(500);
    servo_pump.degree(90);
    delay(500);
}