#define btn_pin 7 // D4

// Left Button: 4 (D4)
// Right Button: 3 (D3)

void setup() {
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(3));
}
