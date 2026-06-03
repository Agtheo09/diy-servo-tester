#include <Servo.h>
#include "Button.h"
#include "BatteryMonitor.h"
#include "ServoController.h"

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

Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);
Button button3(BUTTON_3_PIN);
Button encoderSwitch(ENCODER_SW_PIN);
BatteryMonitor battery(VOLTAGE_PIN, 300);
ServoController myServo1(SERVO_1_PIN, 540, 2200);
ServoController myServo2(SERVO_2_PIN, 540, 2200);

const int ENCODER_STEP = 2;
volatile int encoderValue = 50;
int currentGlobalMode = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    delay(10);
  Serial.println("Serial Init Successful!");

  button1.init();
  button2.init();
  button3.init();
  encoderSwitch.init();
  battery.init();
  myServo1.init();
  myServo2.init();

  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), readEncoder, CHANGE);
  Serial.println("Program Boot Successful!");
}

void loop()
{
  if (button1.isPressed())
  {
    encoderValue = 0;
    beep();
  }

  if (button2.isPressed())
  {
    encoderValue = 100;
    beep();
  }

  if (button3.isPressed())
  {
    currentGlobalMode++;
    if (currentGlobalMode > 2)
      currentGlobalMode = 0;
    myServo1.setMode(currentGlobalMode);
    myServo2.setMode(currentGlobalMode);
    beep();
    delay(50);
    beep();
  }

  if (encoderSwitch.isPressed())
  {
    encoderValue = 50;
    beep();
  }

  battery.update();
  myServo1.update(encoderValue);
  myServo2.update(encoderValue);

  printTelemetry();
}

void beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(30);
  digitalWrite(BUZZER_PIN, LOW);
}

void printTelemetry()
{
  Serial.print("Voltage: ");
  Serial.print(battery.getVoltage(), 2);
  Serial.print("V | Mode: ");
  Serial.print(currentGlobalMode);
  Serial.print(" | Counter Value: ");
  Serial.println(encoderValue);
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