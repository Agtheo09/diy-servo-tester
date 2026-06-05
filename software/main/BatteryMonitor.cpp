#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor(int analogPin, unsigned long checkInterval, float filterFactor)
{
    pin = analogPin;
    interval = checkInterval;
    alpha = filterFactor;   // Assign the custom filtering weight factor
    previousMillis = 0;
    currentVoltage = -1.0; 
}

void BatteryMonitor::update()
{
    unsigned long currentMillis = millis();

    if (currentVoltage < 0.0 || (currentMillis - previousMillis >= interval))
    {
        previousMillis = currentMillis;
        
        int rawAnalog = analogRead(pin);
        float pinVoltage = rawAnalog * (5.0 / 1024.0);
        float instantVoltage = pinVoltage * 3.128; 

        if (currentVoltage < 0.0) {
            currentVoltage = instantVoltage;
        } else {
            // DYNAMIC EMA FILTER: Uses the dynamic alpha parameter passed at setup
            // Example: If alpha is 0.08, then (1.0 - alpha) handles the 0.92 old-value weight
            currentVoltage = (currentVoltage * (1.0 - alpha)) + (instantVoltage * alpha);
        }
    }
}

float BatteryMonitor::getVoltage()
{
    return (currentVoltage < 0.0) ? 0.0 : currentVoltage;
}
