#include <Servo.h>

// --------------------------- Pins --------------------------- //
#define ENCODER_CLK_PIN 2
#define ENCODER_DT_PIN 3
#define ENCODER_SW_PIN A0

#define BUTTON_1_PIN A1
#define BUTTON_2_PIN A2
#define BUTTON_3_PIN A3

#define BUZZER_PIN 4

#define SERVO_1_PIN 5
#define SERVO_2_PIN 6

#define VOLTAGE_PIN A7

// ------------------------- Hardware ------------------------- //
Servo servo1, servo2;

// ------------------------ Variables ------------------------- //
int servoType = 0;     // 0: 180deg, 1: 300deg
int rotation_mode = 0; // 0: Position, 1: Neutral, 2: Swiping
int min_pulse = 540;   // microseconds
int max_pulse = 2200;  // microseconds

// -------------------------- Utils --------------------------- //
bool last_button_1_state = HIGH;
bool last_button_2_state = HIGH;
bool last_button_3_state = HIGH;
bool last_button_rot_state = HIGH;

volatile int encoderValue = 50;
int ENCODER_STEP = 2;
int lastEncoderValue = -1;

unsigned long lastBatteryCheck = 0;
const unsigned long batteryInterval = 300;

int rawAnalog = 0;
float pinVoltage = 0.0;
float batteryVoltage = 0.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Waiting for Serial to INIT...");
  while (!Serial)
    delay(10);
  Serial.println("Serial Init Successful!");

  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);

  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), readEncoder, CHANGE);
  Serial.println("Program Boot Successful!");
}

void loop()
{
  if (digitalRead(BUTTON_1_PIN) == LOW && last_button_1_state == HIGH)
  {
    encoderValue = 0;
    beep();
  }

  if (digitalRead(BUTTON_2_PIN) == LOW && last_button_2_state == HIGH)
  {
    encoderValue = 100;
    beep();
  }

  if (digitalRead(BUTTON_3_PIN) == LOW && last_button_3_state == HIGH)
  {
    delay(10);
  }

  if (digitalRead(ENCODER_SW_PIN) == LOW && last_button_rot_state == HIGH)
  {
    encoderValue = 50;
    beep();
  }

  last_button_1_state = digitalRead(BUTTON_1_PIN);
  last_button_2_state = digitalRead(BUTTON_2_PIN);
  last_button_3_state = digitalRead(BUTTON_3_PIN);
  last_button_rot_state = digitalRead(ENCODER_SW_PIN);

  servo1.writeMicroseconds(map(encoderValue, 0, 100, min_pulse, max_pulse));
  servo2.writeMicroseconds(map(encoderValue, 0, 100, min_pulse, max_pulse));

  if (millis() - lastBatteryCheck >= batteryInterval)
  {

    int rawAnalog = analogRead(VOLTAGE_PIN);
    float pinVoltage = rawAnalog * (5.0 / 1023.0);
    batteryVoltage = pinVoltage * 11.0;
    lastBatteryCheck = millis();
  }

  // --------------------------- Telemetry --------------------------- //
  Serial.print("Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.print("V | Counter Value: ");
  Serial.println(encoderValue);
}

void beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(30);
  digitalWrite(BUZZER_PIN, LOW);
}

void readEncoder()
{
  int clkState = digitalRead(ENCODER_CLK_PIN);
  int dtState = digitalRead(ENCODER_DT_PIN);
  static int lastClkState = HIGH;

  if (clkState != lastClkState && clkState == LOW)
  {
    if (dtState != clkState)
    {
      encoderValue += ENCODER_STEP;
      if (encoderValue > 100)
        encoderValue = 100;
    }
    else
    {
      encoderValue -= ENCODER_STEP;
      if (encoderValue < 0)
        encoderValue = 0;
    }
  }
  lastClkState = clkState;
}
