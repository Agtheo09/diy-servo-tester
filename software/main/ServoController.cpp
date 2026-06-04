#include "ServoController.h"

ServoController::ServoController(int servoPin, int minP, int maxP)
{
    pin = servoPin;
    minPulse = minP;
    maxPulse = maxP;
    mode = 0;
    previousMillis = 0;
    swipeInterval = 10;
    currentPulse = (minP + maxP) / 2;
    swipeDirection = 1;
}

void ServoController::init()
{
    servo.attach(pin);
}

void ServoController::setMode(int newMode)
{
    if (newMode >= 0 && newMode <= 3)
    {
        mode = newMode;
    }
}

void ServoController::setPulseRange(int minP, int maxP)
{
    minPulse = minP;
    maxPulse = maxP;
    currentPulse = (minP + maxP) / 2; // Recenter the servo safely
}

int ServoController::getMode()
{
    return mode;
}

void ServoController::update(int encoderValue)
{
    switch (mode)
    {
    case 0: // MODE 0: Position Mode (Follows the encoder)
    {
        int pulseWidth = map(encoderValue, 0, 100, minPulse, maxPulse);
        servo.writeMicroseconds(pulseWidth);
        break;
    }

    case 1: // MODE 1: Neutral Mode (Locks to exact center)
    {
        int neutralPulse = (minPulse + maxPulse) / 2;
        servo.writeMicroseconds(neutralPulse);
        break;
    }

    case 2: // MODE 2: Swiping Mode (Automated back-and-forth using millis)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= swipeInterval)
        {
            previousMillis = currentMillis;

            currentPulse += (swipeDirection * 8);

            if (currentPulse >= maxPulse)
            {
                currentPulse = maxPulse;
                swipeDirection = -1;
            }
            else if (currentPulse <= minPulse)
            {
                currentPulse = minPulse;
                swipeDirection = 1;
            }

            servo.writeMicroseconds(currentPulse);
        }
        break;
    }
    case 3: // MODE 3:
    {
        // TODO
        break;
    }
    }
}