#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define SERVO_PIN A2
#define POTENSIOMETER_PIN A7
#define MODE_BUTTON_PIN 3 // D3
#define SERVO_TYPE_BUTTON_PIN 4 // D4

Servo the_servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int servoType = 0; // 0: 180deg, 1: 300deg
int rotation_mode = 0; // 0: Position, 1: Neutral, 2: Swiping
int min_pulse = 540; // microseconds
int max_pulse = 2200; // microseconds

// Util
bool last_mode_button_state = HIGH;
bool last_type_button_state = HIGH;

int raw_pot = 0;
int pot_val = 0;
int last_pot_val = 0;
double a = 0.25;

// Swipe things
int pos = 0;
int increment = 1;
unsigned long last_update = 0;
unsigned long interval = 15;

void update_screen() {  
  if(rotation_mode != 1) {
    lcd.setCursor(9, 0);
    char txt[3];
    sprintf(txt, "%03d", map(pot_val, 0, 1023, 0, 100));
    lcd.print(txt);
  } else {
    lcd.setCursor(9, 0);
    lcd.print("   ");
  }
}

void setup() {
  the_servo.attach(SERVO_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SERVO_TYPE_BUTTON_PIN, INPUT_PULLUP);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Press T Button");
  lcd.setCursor(0, 1);
  lcd.print("to begin!");
  while(digitalRead(SERVO_TYPE_BUTTON_PIN) == HIGH);
  while(digitalRead(SERVO_TYPE_BUTTON_PIN) == LOW);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Position 000 %  ");

  lcd.setCursor(0, 1);
  if(servoType == 0) {
    lcd.print("Range: 540-2200");
  } else if(servoType == 1) {
    lcd.print("Range: 500-2500");
  }
}

void loop() {  
  unsigned long currentMillis = millis();

  raw_pot = analogRead(POTENSIOMETER_PIN);
  pot_val = (1-a) * raw_pot + a * pot_val; // 0-1023

  if(abs(pot_val - last_pot_val) > -1) update_screen();

  // Event Handling
  if(digitalRead(MODE_BUTTON_PIN) == LOW && last_mode_button_state == HIGH) {
    // Mode Button Was Pressed
    rotation_mode++; // Swipe through:: 0: Position, 1: Neutral, 2: Swiping
    if(rotation_mode > 2) rotation_mode = 0; // Close the swipe circle

    lcd.setCursor(0, 0);
    if(rotation_mode == 0) {
      lcd.print("Position 000 %  ");
    } else if(rotation_mode == 1) {
      lcd.print("Neutral         ");
    } else {
      lcd.print("Speed    000 %  ");
    }
  }

  if(digitalRead(SERVO_TYPE_BUTTON_PIN) == LOW && last_type_button_state == HIGH) {
    // Servo Type Button Was Pressed
    servoType = servoType == 0 ? 1 : 0; // Switch through:: 0: 180deg, 1: 300deg

    lcd.setCursor(0, 1);
    if(servoType == 0) {
      lcd.print("Range: 540-2200");
    } else if(servoType == 1) {
      lcd.print("Range: 500-2500");
    }
  }

  last_mode_button_state = digitalRead(MODE_BUTTON_PIN);
  last_type_button_state = digitalRead(SERVO_TYPE_BUTTON_PIN);

  if(servoType == 0) {
    min_pulse = 540;
    max_pulse = 2200;
  } else if(servoType == 1) {
    min_pulse = 500;
    max_pulse = 2500;
  }

  if(rotation_mode == 0) {
    the_servo.writeMicroseconds(map(pot_val, 0, 1023, min_pulse, max_pulse));
    lcd.setCursor(0, 0);
  } else if(rotation_mode == 1) {
    the_servo.writeMicroseconds((min_pulse + max_pulse) / 2);
    lcd.setCursor(0, 0);
  } else {
    interval = map(pot_val, 0, 1023, 18, 1); // Change Speed using potensiometer
    
    if (currentMillis - last_update >= interval) {
      last_update = currentMillis;
  
      // Update the servo position
      pos += increment;
  
      // Change the direction at the end of the sweep
      if (pos >= 180 || pos <= 0) {
        increment = -increment;
      }
  
      // Move the servo to the new position
      the_servo.writeMicroseconds(map(pos, 0, 180, min_pulse, max_pulse));
    }
  }

  last_pot_val = pot_val;
}
