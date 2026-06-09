#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class DisplayManager
{
private:
    Adafruit_ILI9341 tft;
    int blPin;

    int lastPercent;
    int lastMode;
    float lastVoltage;
    int lastHighlightedRow;
    int lastInputType;
    int lastServoRange;
    int lastTestSelectionRow;
    int lastOutputCount;
    int lastCellCount;
    int lastLayoutMode;

    void drawStaticMenuLayout();
    void drawStaticTestSelectionLayout();
    void drawStaticTestLayout(int servoRange, int systemState);

public:
    DisplayManager(int csPin, int dcPin, int rstPin, int blPin);
    void init();
    void clearScreen();
    // UPDATED signature footprint to pass cellCount
    void updateMenu(int highlightedRow, int inputType, int cellCount, int servoRange, int outputCount);
    void updateTestSelectionMenu(int highlightedRow);
    void updateTestScreen(int encoderValue, int currentMode, float batteryVoltage, int servoRange, int systemState);
};

#endif
