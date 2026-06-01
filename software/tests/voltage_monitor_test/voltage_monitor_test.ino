#define BATTERY_PIN A7

unsigned long lastBatteryCheck = 0;
const unsigned long batteryInterval = 300;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("--- Non-Blocking Voltage Divider Test Initialized ---");
}

void loop() {
  if (millis() - lastBatteryCheck >= batteryInterval) {
    
    int rawAnalog = analogRead(BATTERY_PIN);
    
    float pinVoltage = rawAnalog * (5.0 / 1023.0);
    
    // New Scaling Factor formula: (10k + 1k) / 1k = 11.0
    float batteryVoltage = pinVoltage * 11.0;

    Serial.print("Raw ADC: ");
    Serial.print(rawAnalog);
    Serial.print(" | Pin Input: ");
    Serial.print(pinVoltage, 2);
    Serial.print("V | Real Battery: ");
    Serial.print(batteryVoltage, 2);
    Serial.println("V");

    lastBatteryCheck = millis();
  }
}
