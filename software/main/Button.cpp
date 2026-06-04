#include "Button.h"

Button::Button(int pinNumber)
{
    pin = pinNumber;
    lastState = HIGH;
}

void Button::init()
{
    pinMode(pin, INPUT_PULLUP);
}

bool Button::isPressed()
{
    bool currentState = digitalRead(pin);
    bool pressed = (currentState == LOW && lastState == HIGH);
    lastState = currentState;
    return pressed;
}