#include   "WaterServo.h"
#include <esp32Servo.h>

Servo servo;

bool WaterServo::begin() 
{
    pinMode(Servopin, OUTPUT);
    servo.attach(Servopin);
    servo.write(90); // Initialize servo to 90 degrees
    return true;
}

void WaterServo::setAngle(int angle) 
{
    static unsigned long lastCommandTime = 0;
        if (millis() - lastCommandTime > 500) {
            lastCommandTime = millis();
            targetAngle = angle;
            update(); // Call update to move the servo
        } 
}

void WaterServo::update() 
{
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