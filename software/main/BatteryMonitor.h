#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

class BatteryMonitor {
private:
    int pin;
    unsigned long interval;
    unsigned long previousMillis;
    float currentVoltage;
    float alpha; // Storage for the dynamic filtering weight factor

public:
    // Added 'filterFactor' argument. Defaults to 0.08 if left blank.
    BatteryMonitor(int analogPin, unsigned long checkInterval, float filterFactor = 0.08);
    void update();
    float getVoltage();
};

#endif
