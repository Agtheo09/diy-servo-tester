#include <Servo.h>
#include "Button.h"
#include "BatteryMonitor.h"
#include "ServoController.h"
#include "DisplayManager.h"

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

#define TFT_CS   10
#define TFT_DC   7   
#define TFT_RST  8
#define TFT_BL   9

Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);
Button button3(BUTTON_3_PIN);
Button encoderSwitch(ENCODER_SW_PIN);
BatteryMonitor battery(VOLTAGE_PIN, 300);
ServoController myServo1(SERVO_1_PIN, 540, 2200);
ServoController myServo2(SERVO_2_PIN, 540, 2200);
DisplayManager display(TFT_CS, TFT_DC, TFT_RST, TFT_BL);

enum SystemState { INIT_MENU, MANUAL_POSITION, NEUTRAL_POSITION, VELOCITY_TEST };
SystemState currentSystemState = INIT_MENU;

int activeMenuRow = 0;   
int configInputType = 0; 
int configServoRange = 0; 

const int ENCODER_STEP = 2;
int encoderValue = 50;    
int currentGlobalMode = 0;

volatile int encoderDelta = 0; 

// --- VELOCITY SELECTION ENGINE VARIABLES ---
static float sweepValue = 0.0;
static int sweepDirection = 1;
static unsigned long lastSweepTick = 0;

void setup()
{
  Serial.begin(9600);
  
  button1.init();
  button2.init();
  button3.init();
  encoderSwitch.init();
  display.init();

  pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), readEncoder, CHANGE);
}

void loop()
{
  battery.update();

  noInterrupts();
  int delta = encoderDelta;
  encoderDelta = 0; 
  interrupts();

  if (currentSystemState == INIT_MENU) 
  {
    if (delta != 0) {
      if (delta > 0) activeMenuRow++;
      else           activeMenuRow--;
      
      if (activeMenuRow > 2) activeMenuRow = 2;
      if (activeMenuRow < 0) activeMenuRow = 0;
    }

    if (encoderSwitch.isPressed()) {
      if (activeMenuRow == 0) {
        configInputType = !configInputType; 
        beep(); 
      } 
      else if (activeMenuRow == 1) {
        configServoRange = !configServoRange; 
        beep(); 
      } 
      else if (activeMenuRow == 2) {
        beep(); delay(60); beep();
        
        if (configServoRange == 0) {
          myServo1.setPulseRange(540, 2200);
          myServo2.setPulseRange(540, 2200);
        } else {
          myServo1.setPulseRange(500, 2500);
          myServo2.setPulseRange(500, 2500);
        }

        myServo1.init();
        myServo2.init();

        display.clearScreen(); 
        encoderValue = 50; 
        currentSystemState = MANUAL_POSITION;
        return;
      }
    }
    
    display.updateMenu(activeMenuRow, configInputType, configServoRange);
  } 
  else 
  {
    // ----------------------------------------------------
    // SYSTEM NAVIGATION MATRIX
    // ----------------------------------------------------
    static unsigned long buttonTimer = 0;
    static bool buttonActive = false;
    static bool longPressTriggered = false;

    if (digitalRead(ENCODER_SW_PIN) == LOW) {
      if (!buttonActive) {
        buttonActive = true;
        buttonTimer = millis();
        longPressTriggered = false;
      } 
      else if (!longPressTriggered && (millis() - buttonTimer > 800)) {
        longPressTriggered = true;
        
        if (currentSystemState == MANUAL_POSITION) {
          currentSystemState = NEUTRAL_POSITION;
        } else if (currentSystemState == NEUTRAL_POSITION) {
          currentSystemState = VELOCITY_TEST;
          encoderValue = 25; 
          sweepValue = 0.0;
          sweepDirection = 1;
        } else if (currentSystemState == VELOCITY_TEST) {
          currentSystemState = MANUAL_POSITION;
          encoderValue = 50; 
        }

        beep(); delay(60); beep(); 
        display.clearScreen();
        return; 
      }
    } 
    else {
      if (buttonActive) {
        buttonActive = false;
        if (!longPressTriggered) {
          if (currentSystemState == MANUAL_POSITION) {
            encoderValue = 50;
            beep();
          } else if (currentSystemState == VELOCITY_TEST) {
            encoderValue = 0; // Quick Brake Stop
            beep();
          }
        }
      }
    }

    // --- STATE UPDATE DRIVERS ---
    int servoOutputValue = 50;

    if (currentSystemState == MANUAL_POSITION) 
    {
      if (delta != 0) encoderValue += (delta * ENCODER_STEP);
      if (encoderValue > 100) encoderValue = 100;
      if (encoderValue < 0)   encoderValue = 0;

      if (button1.isPressed()) { encoderValue = 0; beep(); }
      if (button2.isPressed()) { encoderValue = 100; beep(); }
      
      servoOutputValue = encoderValue;
    } 
    else if (currentSystemState == NEUTRAL_POSITION) 
    {
      encoderValue = 50; 
      servoOutputValue = 50;
    }
    else if (currentSystemState == VELOCITY_TEST)
    {
      if (delta != 0) encoderValue += (delta * ENCODER_STEP);
      if (encoderValue > 100) encoderValue = 100;
      if (encoderValue < 0)   encoderValue = 0;

      if (button1.isPressed()) { encoderValue = 0; beep(); }   
      if (button2.isPressed()) { encoderValue = 100; beep(); } 

      if (encoderValue > 0) {
        // Run updates on a rock-solid physical frame-rate interval
        if (millis() - lastSweepTick >= 15) {
          lastSweepTick = millis();
          
          // RECALIBRATED CORRECTION:
          // Changed scaler multiplier from 0.08 down to 0.03.
          // At 100% speed knob setting, stepSize is now exactly 3.0.
          // 100% travel / 3.0 stepSize = 33 frames * 15ms = ~500ms full sweep.
          // This allows your 270-degree physical servo to perfectly match the software signal!
          float stepSize = (float)encoderValue * 0.03;
          if (stepSize < 0.05) stepSize = 0.05; 

          sweepValue += ((float)sweepDirection * stepSize);
          
          // Pure directional boundary checks without time-based buffers
          if (sweepValue >= 100.0) { 
            sweepValue = 100.0; 
            sweepDirection = -1; 
          }
          else if (sweepValue <= 0.0) { 
            sweepValue = 0.0; 
            sweepDirection = 1; 
          }
        }
      }
      servoOutputValue = (int)sweepValue;
    }

    if (button3.isPressed()) {
      currentGlobalMode++;
      if (currentGlobalMode > 2) currentGlobalMode = 0;
      myServo1.setMode(currentGlobalMode);
      myServo2.setMode(currentGlobalMode);
      beep(); delay(50); beep();
    }

    myServo1.update(servoOutputValue);
    myServo2.update(servoOutputValue);

    display.updateTestScreen(encoderValue, currentGlobalMode, battery.getVoltage(), configServoRange, (int)currentSystemState);
    printTelemetry();
  }
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
  Serial.print("V | State Index: ");
  Serial.print(currentSystemState);
  Serial.print(" | Dial Metric: ");
  Serial.println(encoderValue);
}

void readEncoder()
{
  int clkState = digitalRead(ENCODER_CLK_PIN);
  int dtState = digitalRead(ENCODER_DT_PIN);
  static int lastClkState = HIGH;

  if (clkState != lastClkState && clkState == LOW)
  {
    if (dtState != clkState) { encoderDelta++; }
    else                     { encoderDelta--; }
  }
  lastClkState = clkState;
}
