#include <LiquidCrystal_I2C.h>

// Set the LCD address to your lcd's one! Use an I2c address ester to identify it!
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Turn on the LCD and print a message.
  lcd.setCursor(0, 0);
  lcd.print("Hello!");
  lcd.setCursor(0, 1);
  lcd.print("Servo Tester");
}

void loop()
{
  // Do nothing here...
}
