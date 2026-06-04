#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <Arduino.h>

class BatteryMonitor
{
private:
    int pin;
    unsigned long interval;
    unsigned long previousMillis;
    float currentVoltage;

public:
    BatteryMonitor(int analogPin, unsigned long checkInterval = 300);
    void update();
    float getVoltage();
};

#endif