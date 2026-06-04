#include "DisplayManager.h"
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

DisplayManager::DisplayManager(int csPin, int dcPin, int rstPin, int blPin) 
    : tft(csPin, dcPin, rstPin)
{
    this->blPin = blPin;
    lastPercent = -1; lastMode = -1; lastVoltage = -1.0;
    lastHighlightedRow = -1; lastInputType = -1; lastServoRange = -1;
}

void DisplayManager::init()
{
    pinMode(blPin, OUTPUT);
    digitalWrite(blPin, HIGH);

    tft.begin();
    tft.setRotation(3); 
    tft.fillScreen(ILI9341_BLACK);
    delay(50);
}

void DisplayManager::clearScreen()
{
    tft.fillScreen(ILI9341_BLACK);
    lastPercent = -1; lastMode = -1; lastVoltage = -1.0;
    lastHighlightedRow = -1; lastInputType = -1; lastServoRange = -1;
}

void DisplayManager::drawStaticMenuLayout()
{
    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(1);
    
    tft.drawFastHLine(0, 24, tft.width(), ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(15, 17);
    tft.print("SYSTEM INITIALIZATION");

    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setCursor(30, 70);  tft.print("Input Source:");
    tft.setCursor(30, 120); tft.print("Servo Range:");
}

void DisplayManager::updateMenu(int highlightedRow, int inputType, int servoRange)
{
    if (lastHighlightedRow == -1) {
        drawStaticMenuLayout();
    }

    int w = tft.width();
    tft.setFont(&FreeSansBold9pt7b);

    if (inputType != lastInputType) {
        tft.fillRect(175, 50, 125, 26, ILI9341_BLACK); 
        tft.setCursor(180, 70);
        if (inputType == 0) { tft.setTextColor(ILI9341_YELLOW); tft.print("BATTERY"); }
        else                { tft.setTextColor(ILI9341_CYAN);   tft.print("POWER SUP."); }
        lastInputType = inputType;
    }

    if (servoRange != lastServoRange) {
        tft.fillRect(175, 100, 125, 26, ILI9341_BLACK); 
        tft.setCursor(180, 120);
        if (servoRange == 0) { tft.setTextColor(ILI9341_MAGENTA); tft.print("540 - 2200"); }
        else                 { tft.setTextColor(ILI9341_ORANGE);  tft.print("500 - 2500"); }
        lastServoRange = servoRange;
    }

    if (highlightedRow != lastHighlightedRow) {
        for (int i = 0; i < 2; i++) {
            int targetY = 48 + (i * 50);
            tft.drawRoundRect(15, targetY, w - 30, 32, 6, ILI9341_BLACK);
        }
        
        tft.fillRoundRect(40, 175, w - 80, 32, 16, ILI9341_BLACK);
        tft.drawRoundRect(40, 175, w - 80, 32, 16, ILI9341_GREEN);

        if (highlightedRow < 2) {
            int newTargetY = 48 + (highlightedRow * 50);
            tft.drawRoundRect(15, newTargetY, w - 30, 32, 6, ILI9341_WHITE);

            tft.setFont(&FreeSansBold9pt7b);
            tft.setCursor((w / 2) - 55, 196);
            tft.setTextColor(ILI9341_GREEN);
            tft.print("START TEST");
        } 
        else {
            tft.fillRoundRect(40, 175, w - 80, 32, 16, ILI9341_GREEN);
            tft.setFont(&FreeSansBold9pt7b);
            tft.setCursor((w / 2) - 55, 196);
            tft.setTextColor(ILI9341_BLACK); 
            tft.print("START TEST");
        }

        lastHighlightedRow = highlightedRow;
    }
}

void DisplayManager::drawStaticTestLayout(int servoRange, int systemState)
{
    int w = tft.width();
    int h = tft.height();

    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(1);

    int headerLineY = h * 0.10; 
    tft.drawFastHLine(0, headerLineY, w, ILI9341_DARKGREY);
    
    tft.setTextColor(ILI9341_MAGENTA); 
    tft.setCursor(15, 17); 
    
    // --- ADDED VELOCITY STATE HEADER ROUTING ---
    if (systemState == 1)      tft.print("MANUAL POSITION");
    else if (systemState == 2) tft.print("NEUTRAL POSITION");
    else if (systemState == 3) tft.print("VELOCITY TEST");
    
    int footerLineY = h * 0.90; 
    tft.drawFastHLine(0, footerLineY, w, ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_CYAN);
    int slotWidth = w / 3;
    int footerTextY = 233; 
    
    tft.setCursor((slotWidth * 0) + (slotWidth / 2) - 25, footerTextY); tft.print("BTN 1");  
    tft.setCursor((slotWidth * 1) + (slotWidth / 2) - 25, footerTextY); tft.print("BTN 2"); 
    tft.setCursor((slotWidth * 2) + (slotWidth / 2) - 25, footerTextY); tft.print("BTN 3");  

    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(ILI9341_GREEN);
    tft.setCursor((w / 2) + 12, 80); 
    tft.print("%");

    int barWidth = w * 0.80; int barHeight = 30;
    int barX = (w - barWidth) / 2; 
    int barY = h * 0.48;     
    tft.drawRect(barX - 2, barY - 2, barWidth + 4, barHeight + 4, ILI9341_WHITE);

    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setCursor(102, 170); 
    if (servoRange == 0) tft.print("540 - 2200 us");
    else                 tft.print("500 - 2500 us");
}

void DisplayManager::updateTestScreen(int encoderValue, int currentMode, float batteryVoltage, int servoRange, int systemState)
{
    if (lastPercent == -1) {
        drawStaticTestLayout(servoRange, systemState);
    }

    int w = tft.width();
    int h = tft.height();

    if (batteryVoltage >= lastVoltage + 0.05 || batteryVoltage <= lastVoltage - 0.05 || lastVoltage < 0) {
        tft.setFont(&FreeSansBold9pt7b);
        tft.fillRect(w - 15 - 65, 2, 70, 20, ILI9341_BLACK); 
        tft.setTextColor(ILI9341_YELLOW); 
        tft.setCursor(w - 15 - 55, 17);
        tft.print(batteryVoltage, 2);
        tft.print("V");
        lastVoltage = batteryVoltage;
    }

    if (currentMode != lastMode) {
        tft.setFont(&FreeSansBold9pt7b);
        tft.fillRect(230, 35, 85, 20, ILI9341_BLACK); 
        tft.setCursor(230, 50);
        switch (currentMode) {
            case 0: break;
            case 1: tft.setTextColor(ILI9341_BLUE);    tft.print("[CENTER]"); break;
            case 2: tft.setTextColor(ILI9341_ORANGE);  tft.print("[SWEEP]");  break;
        }
        lastMode = currentMode;
    }

    if (encoderValue != lastPercent) {
        tft.setFont(&FreeSansBold18pt7b);
        tft.fillRect((w / 2) - 65, 47, 72, 36, ILI9341_BLACK);

        char numStr[4];
        sprintf(numStr, "%d", encoderValue);

        int16_t x1, y1; uint16_t textW, textH;
        tft.getTextBounds(numStr, 0, 0, &x1, &y1, &textW, &textH);
        int dynamicNumX = (w / 2) + 4 - textW;

        tft.setCursor(dynamicNumX, 80);
        tft.setTextColor(ILI9341_GREEN);
        tft.print(numStr);

        int barWidth = w * 0.80; int barHeight = 30;
        int barX = (w - barWidth) / 2; 
        int barY = h * 0.48;     
        int dynamicPixelFill = map(encoderValue, 0, 100, 0, barWidth);
        
        if (dynamicPixelFill > 0) {
            tft.fillRect(barX, barY, dynamicPixelFill, barHeight, ILI9341_GREEN);
        }
        if (dynamicPixelFill < barWidth) {
            tft.fillRect(barX + dynamicPixelFill, barY, barWidth - dynamicPixelFill, barHeight, ILI9341_BLACK);
        }
        lastPercent = encoderValue;
    }
}
