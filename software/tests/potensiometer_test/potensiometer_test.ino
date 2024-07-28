void setup() {
  pinMode(A7, INPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(A7);
  Serial.println(potValue);
}
