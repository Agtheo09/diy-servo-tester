#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x3E for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Turn on the LCD and print a message.
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("PEOS");
}

void loop() {
  // Do nothing here...
}
