#include <Servo.h>

#define SERVO_PIN A2
#define POTENSIOMETER_PIN A7
#define MODE_BUTTON_PIN 3 // D3
#define SERVO_TYPE_BUTTON_PIN 4 // D4

Servo the_servo;

int servoType = 0; // 0: 180deg, 1: 300deg
int rotation_mode = 0; // 0: Position, 1: Neutral, 2: Swiping
int min_pulse = 800; // microseconds
int max_pulse = 2200; // microseconds

// Util
bool last_mode_button_state = HIGH;
bool last_type_button_state = HIGH;

// Swipe things
int pos = 0;
int increment = 1;
unsigned long last_update = 0;
unsigned long interval = 15;

void setup() {
  the_servo.attach(SERVO_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SERVO_TYPE_BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  int pot_val = analogRead(POTENSIOMETER_PIN); // 0-1023

  // Event Handling
  if(digitalRead(MODE_BUTTON_PIN) == LOW && last_mode_button_state == HIGH) {
    // Mode Button Was Pressed
    rotation_mode++; // Swipe through:: 0: Position, 1: Neutral, 2: Swiping
    if(rotation_mode > 2) rotation_mode = 0; // Close the swipe circle
  }

  if(digitalRead(SERVO_TYPE_BUTTON_PIN) == LOW && last_type_button_state == HIGH) {
    // Servo Type Button Was Pressed
    servoType = servoType == 0 ? 1 : 0; // Switch through:: 0: 180deg, 1: 300deg
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
  } else if(rotation_mode == 1) {
    the_servo.writeMicroseconds((min_pulse + max_pulse) / 2);
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

  Serial.println(rotation_mode);
}
