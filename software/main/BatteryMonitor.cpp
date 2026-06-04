#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor(int analogPin, unsigned long checkInterval)
{
    pin = analogPin;
    interval = checkInterval;
    previousMillis = 0;
    currentVoltage = 0.0;
}

void BatteryMonitor::update()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        int rawAnalog = analogRead(pin);
        float pinVoltage = rawAnalog * (5.0 / 1023.0);
        currentVoltage = pinVoltage * 11.0; // Voltage divider ratio factor
    }
}

float BatteryMonitor::getVoltage()
{
    return currentVoltage;
}