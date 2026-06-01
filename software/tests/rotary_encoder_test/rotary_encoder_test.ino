// Updated Encoder Pins based on your new choices
#define ENCODER_CLK 2  // Interrupt Pin 0
#define ENCODER_DT  3  // Interrupt Pin 1
#define ENCODER_SW  A0 // Using A0 as a digital input pin

volatile int currentCounter = 115; // 'volatile' is mandatory for variables modified inside interrupts
unsigned long lastButtonPress = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Interrupt-Driven Encoder Test Initialized ---");

  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP); // Activates pull-up on A0

  // Attach interrupts to trigger the readEncoder function whenever CLK changes state
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, CHANGE);
}

void loop() {
  // The main loop doesn't have to watch the dial at all anymore!
  // We just check the counter whenever we want to print it.
  
  static int lastPrintedCounter = -1;
  if (currentCounter != lastPrintedCounter) {
    Serial.print("Position Value: ");
    Serial.println(currentCounter);
    lastPrintedCounter = currentCounter;
  }

  // Read Button Click on A0 (With 50ms software debounce)
  if (digitalRead(ENCODER_SW) == LOW) {
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button Pressed! Resetting to 115°...");
      currentCounter = 115;
    }
    lastButtonPress = millis();
  }
}

// This special function runs instantly in the background the exact millisecond the dial moves
void readEncoder() {
  int clkState = digitalRead(ENCODER_CLK);
  int dtState = digitalRead(ENCODER_DT);
  
  // Static variable remembers the last state across interrupt triggers
  static int lastClkState = HIGH; 

  if (clkState != lastClkState && clkState == LOW) {
    if (dtState != clkState) {
      currentCounter++;
      if (currentCounter > 180) currentCounter = 180;
    } else {
      currentCounter--;
      if (currentCounter < 0) currentCounter = 0;
    }
  }
  lastClkState = clkState;
}
