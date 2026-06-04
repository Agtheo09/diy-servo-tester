#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
private:
    int pin;
    bool lastState;

public:
    Button(int pinNumber);
    void init();
    bool isPressed(); // Returns true ONLY on the exact transition from HIGH to LOW
};

#endif