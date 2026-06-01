// --- PIN CONFIGURATIONS ---
#define ENCODER_CLK 2   // Interrupt Pin 0
#define ENCODER_DT  3   // Interrupt Pin 1
#define ENCODER_SW  A0  // Encoder built-in click switch
#define BUTTON_1    A1  // Extra Button 1
#define BUTTON_2    A2  // Extra Button 2
#define BUZZER    4

// --- GLOBAL VARIABLES ---
volatile int counterValue = 115; // 'volatile' is required for variables updated in interrupts
int lastPrintedCounter = -1;
unsigned long lastButtonPress = 0;

void setup() {
  // Initialize Serial Monitor for testing
  Serial.begin(9600);
  while (!Serial) { ; } 
  Serial.println("--- Controls Test: Encoder + 2 Buttons ---");

  // Activate internal pull-ups (Pins read HIGH normally, LOW when pressed)
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  // Attach background hardware interrupt for instant dial tracking
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, CHANGE);
}

void loop() {
  // 1. MONITOR BUTTON 1
  if (digitalRead(BUTTON_1) == LOW) {
    Serial.println("Button 1 Pressed! Snapping counter to 0...");
    counterValue = 0;
    delay(150); // Simple debounce delay for testing
  }
  
  // 2. MONITOR BUTTON 2
  if (digitalRead(BUTTON_2) == LOW) {
    Serial.println("Button 2 Pressed! Snapping counter to 180...");
    counterValue = 180;
    delay(150); // Simple debounce delay for testing
  }

  // 3. MONITOR ENCODER DIAL CLICK
  if (digitalRead(ENCODER_SW) == LOW) {
    if (millis() - lastButtonPress > 200) { // 200ms software debounce
      Serial.println("Encoder Knob Clicked! Resetting to home (115)...");
      counterValue = 115;
      lastButtonPress = millis();
      digitalWrite(BUZZER, HIGH);
      delay(30);
      digitalWrite(BUZZER, LOW);
    }
  }

  // 4. PRINT VALUE CHANGES IMMEDIATELY
  if (counterValue != lastPrintedCounter) {
    Serial.print("Counter Value: ");
    Serial.println(counterValue);
    lastPrintedCounter = counterValue;
  }
}

// Background Hardware Interrupt Routine (Fires instantly on dial movement)
void readEncoder() {
  int clkState = digitalRead(ENCODER_CLK);
  int dtState = digitalRead(ENCODER_DT);
  static int lastClkState = HIGH; 

  if (clkState != lastClkState && clkState == LOW) {
    if (dtState != clkState) {
      counterValue++;
      if (counterValue > 180) counterValue = 180; // Upper limit
    } else {
      counterValue--;
      if (counterValue < 0) counterValue = 0;     // Lower limit
    }
  }
  lastClkState = clkState;
}
