#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Include the smoother, premium custom fonts
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

// --- WAVESHARE PIN CONFIGURATION ---
#define TFT_CS   10
#define TFT_DC   7   
#define TFT_RST  8
#define TFT_BL   9

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

float animationAngle = 0.0; 
int currentPercent = 0;
int lastPercent = -1; 

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.begin();
  tft.setRotation(1); // Landscape (320x240)
  tft.fillScreen(ILI9341_BLACK);
  delay(50);
  
  drawStaticLayout();
}

void drawStaticLayout() {
  int w = tft.width();
  int h = tft.height();

  // Set smooth font for headers and footers
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1); // Custom fonts should stay at size 1 for maximum smoothness

  // ---------------- HEADER ZONE ----------------
  int headerLineY = h * 0.10; 
  tft.drawFastHLine(0, headerLineY, w, ILI9341_DARKGREY);
  
  // Custom fonts render from the baseline up, so Y coordinates are adjusted down
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(15, 17); 
  tft.print("SERVO TESTER");
  
  tft.setTextColor(ILI9341_YELLOW); 
  tft.setCursor(w - 15 - 55, 17);
  tft.print("5.08V");
  
  // ---------------- FOOTER ZONE ----------------
  int footerLineY = h * 0.90; 
  tft.drawFastHLine(0, footerLineY, w, ILI9341_DARKGREY);
  
  tft.setTextColor(ILI9341_CYAN);
  int slotWidth = w / 3;
  int footerTextY = 233; 
  
  tft.setCursor((slotWidth * 0) + (slotWidth / 2) - 24, footerTextY);
  tft.print("MODE");  
  
  tft.setCursor((slotWidth * 1) + (slotWidth / 2) - 24, footerTextY);
  tft.print("DOWN"); 
  
  tft.setCursor((slotWidth * 2) + (slotWidth / 2) - 12, footerTextY);
  tft.print("UP");  

  // ---------------- STATIC PERCENT SIGN ----------------
  // We draw the large % sign ONCE right here. It will never flicker or rewrite.
  tft.setFont(&FreeSansBold18pt7b);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor((w / 2) + 12, 95); // Fixed position slightly right of center
  tft.print("%");

  // ---------------- CENTER PROGRESS BAR CONTAINER ----------------
  int barWidth = w * 0.80; 
  int barHeight = 30;
  int barX = (w - barWidth) / 2;
  int barY = h * 0.55;     
  
  tft.drawRect(barX - 2, barY - 2, barWidth + 4, barHeight + 4, ILI9341_WHITE);
}

void loop() {
  animationAngle += 0.04; 
  if (animationAngle >= 2 * PI) {  
    animationAngle -= 2 * PI;
  }
  currentPercent = (0.5 - 0.5 * cos(animationAngle)) * 100.0;

  // FILTER: Skip rendering if data hasn't changed
  if (currentPercent == lastPercent) {
    delay(10); 
    return;
  }
  
  lastPercent = currentPercent;

  int w = tft.width();
  int h = tft.height();
  int barY = h * 0.55;
  int barWidth = w * 0.80;
  int barHeight = 30;
  int barX = (w - barWidth) / 2;

  // 1. LIVE NUMBER ONLY UPDATE (Zero Jitter Architecture)
  tft.setFont(&FreeSansBold18pt7b);
  
  // Clear ONLY the bounding box where the numbers sit (stops before the % sign)
  // Box spans from X: (w/2)-65 to (w/2)+5. The % sign safely lives at (w/2)+12.
  tft.fillRect((w / 2) - 65, 62, 72, 36, ILI9341_BLACK);

  // Convert percentage integer to string
  char numStr[4];
  sprintf(numStr, "%d", currentPercent);

  // Measure string width dynamically to handle proportional font widths
  int16_t x1, y1;
  uint16_t textW, textH;
  tft.getTextBounds(numStr, 0, 0, &x1, &y1, &textW, &textH);

  // Right-align calculations: Anchor the right edge of the numbers perfectly at (w/2) + 4
  int dynamicNumX = (w / 2) + 4 - textW;

  tft.setCursor(dynamicNumX, 95);
  tft.setTextColor(ILI9341_GREEN);
  tft.print(numStr);

  // 2. LIVE CENTER BAR UPDATE
  int dynamicPixelFill = map(currentPercent, 0, 100, 0, barWidth);
  
  if (dynamicPixelFill > 0) {
    tft.fillRect(barX, barY, dynamicPixelFill, barHeight, ILI9341_GREEN);
  }
  if (dynamicPixelFill < barWidth) {
    tft.fillRect(barX + dynamicPixelFill, barY, barWidth - dynamicPixelFill, barHeight, ILI9341_BLACK);
  }

  delay(5); 
}
