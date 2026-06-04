#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class DisplayManager {
private:
    Adafruit_ILI9341 tft;
    int blPin;

    // Cache states to avoid screen flickering
    int lastPercent;
    int lastMode;
    float lastVoltage;
    int lastHighlightedRow;
    int lastInputType;
    int lastServoRange;

    // Both private and public layouts must accept systemState
    void drawStaticTestLayout(int servoRange, int systemState); 
    void drawStaticMenuLayout();

public:
    DisplayManager(int csPin, int dcPin, int rstPin, int blPin);
    void init();
    void clearScreen();
    void updateMenu(int highlightedRow, int inputType, int servoRange);
    void updateTestScreen(int encoderValue, int currentMode, float batteryVoltage, int servoRange, int systemState);
};

#endif
