#define btn_pin 7

void setup()
{
  pinMode(4, INPUT_PULLUP);
  // Initialize Serial Monitor for testing
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  Serial.println("--- Controls Test: Button ---");
}

void loop()
{
  Serial.println(digitalRead(4));
}
