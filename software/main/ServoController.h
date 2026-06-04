#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

class ServoController
{
private:
    Servo servo;
    int pin;
    int minPulse;
    int maxPulse;
    int mode; // 0: Position, 1: Neutral, 2: Swiping

    // Variables for non-blocking automatic swiping
    unsigned long previousMillis;
    unsigned long swipeInterval; // How fast it sweeps (ms per step)
    int currentPulse;
    int swipeDirection; // 1 = moving up, -1 = moving down

public:
    ServoController(int servoPin, int minP = 540, int maxP = 2200);
    void init();
    void setMode(int newMode);
    int getMode();
    void update(int encoderValue); // Core logic ran every frame
    void setPulseRange(int minP, int maxP);
};

#endif