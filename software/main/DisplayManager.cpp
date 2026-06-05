#include "DisplayManager.h"

static int lastOutputCount = -1; 

DisplayManager::DisplayManager(int csPin, int dcPin, int rstPin, int blPin) 
    : tft(csPin, dcPin, rstPin)
{
    this->blPin = blPin;
    lastPercent = -1; lastMode = -1; lastVoltage = -1.0;
    lastHighlightedRow = -1; lastInputType = -1; lastServoRange = -1;
    lastTestSelectionRow = -1;
    lastOutputCount = -1;
    lastCellCount = -1;
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
    lastTestSelectionRow = -1;
    lastOutputCount = -1;
    lastCellCount = -1;
}

uint16_t getThemeColor(int stateOrRow, bool isSystemState) 
{
    int index = isSystemState ? (stateOrRow - 2) : stateOrRow;
    switch(index) {
        case 0:  return ILI9341_CYAN;         
        case 1:  return ILI9341_RED;       // Switched to match new system identity
        case 2:  return ILI9341_ORANGE;       
        case 3:  return ILI9341_PINK;         
        default: return ILI9341_WHITE;
    }
}

void DisplayManager::drawStaticMenuLayout() {}

void DisplayManager::updateMenu(int highlightedRow, int inputType, int cellCount, int servoRange, int outputCount)
{
    int w = tft.width();
    
    // Step 1: Render background base layout frame strictly once
    if (lastHighlightedRow == -1) {
        tft.setTextSize(2);
        tft.drawFastHLine(0, 26, w, ILI9341_DARKGREY);
        tft.setTextColor(ILI9341_WHITE);
        tft.setCursor(15, 6);
        tft.print("SYSTEM INITIALIZATION");

        tft.setTextColor(ILI9341_LIGHTGREY);
        tft.setCursor(25, 42);  tft.print("Input Source:");
        tft.setCursor(25, 77);  tft.print("Cell Count:");
        tft.setCursor(25, 112); tft.print("Servo Range:");
        tft.setCursor(25, 147); tft.print("Output Mode:");

        // Updated text to "START" and centered the text string layout safely
        tft.drawRoundRect(40, 184, w - 80, 32, 16, ILI9341_GREEN);
        tft.setCursor((w / 2) - 30, 192);
        tft.setTextColor(ILI9341_GREEN);
        tft.print("START");
    }

    tft.setTextSize(2);

    // Step 2: Handle Input Context Shifts (Corrected clearing box constraints)
    if (inputType != lastInputType) {
        tft.fillRect(185, 38, 110, 22, ILI9341_BLACK); // Fixed boundary width to prevent border clipping
        tft.setCursor(185, 42);
        switch(inputType) {
            case 0: tft.setTextColor(ILI9341_CYAN);   tft.print("PWR SUP."); break;
            case 1: tft.setTextColor(ILI9341_RED);    tft.print("LiPo");     break; // Color change: Red
            case 2: tft.setTextColor(ILI9341_ORANGE); tft.print("NiMH");     break; // Color change: Orange
        }

        tft.setCursor(25, 77);
        if (inputType == 0) {
            tft.setTextColor(ILI9341_DARKGREY);
            tft.print("Cell Count:");
            tft.fillRect(185, 73, 110, 22, ILI9341_BLACK);
            tft.setCursor(185, 77);
            tft.print("N/A");
        } else {
            tft.setTextColor(ILI9341_LIGHTGREY);
            tft.print("Cell Count:");
            lastCellCount = -1; // Force a refresh to adapt new theme colors safely
        }
        lastInputType = inputType;
    }

    // Colors cell text dynamically to match source battery style profile
    if (inputType > 0 && (cellCount != lastCellCount || lastCellCount == -1)) {
        tft.fillRect(185, 73, 110, 22, ILI9341_BLACK);
        tft.setCursor(185, 77);
        
        if (inputType == 1)      tft.setTextColor(ILI9341_RED);    // LiPo cells -> Red
        else if (inputType == 2) tft.setTextColor(ILI9341_ORANGE); // NiMH cells -> Orange
        
        tft.print(cellCount);
        tft.print("S");
        lastCellCount = cellCount;
    }

    if (servoRange != lastServoRange) {
        tft.fillRect(185, 108, 110, 22, ILI9341_BLACK); 
        tft.setCursor(185, 112);
        if (servoRange == 0) { tft.setTextColor(ILI9341_PINK);   tft.print("540-2200"); }
        else                 { tft.setTextColor(ILI9341_ORANGE); tft.print("500-2500"); }
        lastServoRange = servoRange;
    }

    if (outputCount != lastOutputCount) {
        tft.fillRect(185, 143, 110, 22, ILI9341_BLACK);
        tft.setCursor(185, 147);
        if (outputCount == 0) { tft.setTextColor(ILI9341_WHITE); tft.print("SINGLE"); }
        else                  { tft.setTextColor(ILI9341_GREEN); tft.print("DUAL"); }
        lastOutputCount = outputCount;
    }

    // Step 3: Selector Box Shifting Engine
    if (highlightedRow != lastHighlightedRow) {
        if (lastHighlightedRow >= 0 && lastHighlightedRow <= 3) {
            int oldY = 36 + (lastHighlightedRow * 35);
            tft.drawRoundRect(15, oldY, w - 30, 28, 6, ILI9341_BLACK);
        }
        
        if (highlightedRow <= 3) {
            int newY = 36 + (highlightedRow * 35);
            tft.drawRoundRect(15, newY, w - 30, 28, 6, ILI9341_WHITE);
        }

        bool wasButtonActive = (lastHighlightedRow == 4);
        bool isButtonActive = (highlightedRow == 4);

        if (lastHighlightedRow == -1 || (wasButtonActive != isButtonActive)) {
            if (isButtonActive) {
                tft.fillRoundRect(40, 184, w - 80, 32, 16, ILI9341_GREEN);
                tft.setCursor((w / 2) - 30, 192);
                tft.setTextColor(ILI9341_BLACK); 
                tft.print("START");
            } else {
                tft.fillRoundRect(40, 184, w - 80, 32, 16, ILI9341_BLACK);
                tft.drawRoundRect(40, 184, w - 80, 32, 16, ILI9341_GREEN);
                tft.setCursor((w / 2) - 30, 192);
                tft.setTextColor(ILI9341_GREEN);
                tft.print("START");
            }
        }
        lastHighlightedRow = highlightedRow;
    }
}

void DisplayManager::drawStaticTestSelectionLayout()
{
    tft.setTextSize(2);
    tft.drawFastHLine(0, 26, tft.width(), ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(15, 6);
    tft.print("SELECT TEST MODE");
}

void DisplayManager::updateTestSelectionMenu(int highlightedRow)
{
    int w = tft.width();
    tft.setTextSize(2);

    const char* testList[] = {
        "1. MANUAL POSITION",
        "2. NEUTRAL POSITION",
        "3. VELOCITY SWEEP",
        "4. STAMPS"
    };

    if (lastTestSelectionRow == -1) {
        drawStaticTestSelectionLayout();
        for (int i = 0; i < 4; i++) {
            int targetY = 36 + (i * 45);
            uint16_t modeColor = getThemeColor(i, false);

            tft.fillRoundRect(20, targetY, w - 40, 34, 6, ILI9341_BLACK);
            if (i == highlightedRow) {
                tft.drawRoundRect(20, targetY, w - 40, 34, 6, modeColor);
                tft.setTextColor(modeColor);
            } else {
                tft.drawRoundRect(20, targetY, w - 40, 34, 6, ILI9341_DARKGREY);
                tft.setTextColor(ILI9341_LIGHTGREY);
            }
            tft.setCursor(35, targetY + 9);
            tft.print(testList[i]);
        }
        lastTestSelectionRow = highlightedRow;
    } 
    else if (highlightedRow != lastTestSelectionRow) {
        if (lastTestSelectionRow >= 0 && lastTestSelectionRow < 4) {
            int oldY = 36 + (lastTestSelectionRow * 45);
            tft.drawRoundRect(20, oldY, w - 40, 34, 6, ILI9341_DARKGREY);
            tft.setTextColor(ILI9341_LIGHTGREY);
            tft.setCursor(35, oldY + 9);
            tft.print(testList[lastTestSelectionRow]);
        }

        int newY = 36 + (highlightedRow * 45);
        uint16_t activeColor = getThemeColor(highlightedRow, false);
        tft.drawRoundRect(20, newY, w - 40, 34, 6, activeColor);
        tft.setTextColor(activeColor);
        tft.setCursor(35, newY + 9);
        tft.print(testList[highlightedRow]);

        lastTestSelectionRow = highlightedRow;
    }
}

void DisplayManager::drawStaticTestLayout(int servoRange, int systemState)
{
    int w = tft.width();
    int h = tft.height();

    uint16_t testAccentColor = getThemeColor(systemState, true);

    tft.setTextSize(2);
    int headerLineY = h * 0.12; 
    tft.drawFastHLine(0, headerLineY, w, ILI9341_DARKGREY);
    
    tft.setTextColor(testAccentColor); 
    tft.setCursor(15, 6); 
    
    if (systemState == 2)      tft.print("MANUAL POSITION");
    else if (systemState == 3) tft.print("NEUTRAL POSITION");
    else if (systemState == 4) tft.print("VELOCITY TEST");
    else if (systemState == 5) tft.print("STAMPS");
    
    int footerLineY = h * 0.88; 
    tft.drawFastHLine(0, footerLineY, w, ILI9341_DARKGREY);
    
    tft.setTextColor(ILI9341_WHITE);
    int slotWidth = w / 3;
    int footerTextY = 218; 
    
    tft.setCursor((slotWidth * 0) + (slotWidth / 2) - 30, footerTextY); tft.print("BTN 1");  
    tft.setCursor((slotWidth * 1) + (slotWidth / 2) - 30, footerTextY); tft.print("BTN 2"); 
    tft.setCursor((slotWidth * 2) + (slotWidth / 2) - 30, footerTextY); tft.print("BTN 3");  

    tft.setTextSize(4); 
    tft.setTextColor(testAccentColor);
    tft.setCursor((w / 2) + 25, 52); 
    tft.print("%");

    int barWidth = w * 0.80; int barHeight = 30;
    int barX = (w - barWidth) / 2; 
    int barY = h * 0.48;     
    tft.drawRect(barX - 2, barY - 2, barWidth + 4, barHeight + 4, ILI9341_WHITE);

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setCursor(90, 160); 
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
    uint16_t testAccentColor = getThemeColor(systemState, true);

    if (batteryVoltage >= lastVoltage + 0.05 || batteryVoltage <= lastVoltage - 0.05 || lastVoltage < 0) {
        tft.setTextSize(2);
        tft.fillRect(w - 15 - 75, 4, 80, 20, ILI9341_BLACK); 
        
        tft.setTextColor(ILI9341_LIGHTGREY); 
        tft.setCursor(w - 15 - 70, 6);
        tft.print(batteryVoltage, 2);
        tft.print("V");
        lastVoltage = batteryVoltage;
    }

    if (currentMode != lastMode) {
        tft.setTextSize(1); 
        tft.fillRect(230, 33, 85, 12, ILI9341_BLACK); 
        tft.setCursor(230, 35);
        switch (currentMode) {
            case 0: break;
            case 1: tft.setTextColor(ILI9341_BLUE);   tft.print("[CENTER]"); break;
            case 2: tft.setTextColor(ILI9341_YELLOW); tft.print("[SWEEP]");  break;
        }
        lastMode = currentMode;
    }

    if (encoderValue != lastPercent) {
        tft.setTextSize(4);
        tft.fillRect((w / 2) - 75, 52, 95, 32, ILI9341_BLACK);

        char numStr[4];
        itoa(encoderValue, numStr, 10); 

        int stringLen = strlen(numStr);
        int dynamicNumX = (w / 2) + 15 - (stringLen * 24); 

        tft.setCursor(dynamicNumX, 52);
        tft.setTextColor(testAccentColor);
        tft.print(numStr);

        int barWidth = w * 0.80; int barHeight = 30;
        int barX = (w - barWidth) / 2; 
        int barY = h * 0.48;     
        int dynamicPixelFill = map(encoderValue, 0, 100, 0, barWidth);
        
        if (dynamicPixelFill > 0) {
            tft.fillRect(barX, barY, dynamicPixelFill, barHeight, testAccentColor);
        }
        if (dynamicPixelFill < barWidth) {
            tft.fillRect(barX + dynamicPixelFill, barY, barWidth - dynamicPixelFill, barHeight, ILI9341_BLACK);
        }
        lastPercent = encoderValue;
    }
}
