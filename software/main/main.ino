#include <Servo.h>
#include <Encoder.h>
#include "Button.h"
#include "BatteryMonitor.h"
#include "ServoController.h"
#include "DisplayManager.h"

#define DEBUG_TELEMETRY 1 

#define ENCODER_CLK_PIN 3
#define ENCODER_DT_PIN 2
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
BatteryMonitor battery(VOLTAGE_PIN, 300, 0.08); 
ServoController myServo1(SERVO_1_PIN, 540, 2200);
ServoController myServo2(SERVO_2_PIN, 540, 2200);
DisplayManager display(TFT_CS, TFT_DC, TFT_RST, TFT_BL);

Encoder myHardwareEnc(ENCODER_CLK_PIN, ENCODER_DT_PIN);
long lastNormalizedPos = 0;

enum SystemState { INIT_MENU, TEST_SELECTION_MENU, MANUAL_POSITION, NEUTRAL_POSITION, VELOCITY_TEST, STAMPS };
SystemState currentSystemState = INIT_MENU;

int activeMenuRow = 0;       
int activeTestMenuRow = 0;   
int configInputType = 0;   // 0=PWR SUP, 1=LiPo, 2=NiMH
int configCellCount = 0;   
int configServoRange = 0; 
int configOutputCount = 1; 

const int ENCODER_STEP = 2;
int encoderValue = 50;    
int currentGlobalMode = 0;

static float sweepValue = 0.0;
static int sweepDirection = 1;
static unsigned long lastSweepTick = 0;

void setup()
{
  #if DEBUG_TELEMETRY
    Serial.begin(9600);
  #endif
  
  button1.init();
  button2.init();
  button3.init();
  encoderSwitch.init();
  display.init();

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  battery.update();

  long currentNormalizedPos = myHardwareEnc.read() / 4; 
  int delta = currentNormalizedPos - lastNormalizedPos;
  lastNormalizedPos = currentNormalizedPos;

  if (currentSystemState == INIT_MENU) 
  {
    if (delta != 0) {
      if (delta > 0) {
        activeMenuRow++;
        if (configInputType == 0 && activeMenuRow == 1) activeMenuRow = 2;
      } else {
        activeMenuRow--;
        if (configInputType == 0 && activeMenuRow == 1) activeMenuRow = 0;
      }
      if (activeMenuRow > 4) activeMenuRow = 4;
      if (activeMenuRow < 0) activeMenuRow = 0;
    }

    if (encoderSwitch.isPressed()) {
      
      if (activeMenuRow == 0) { 
        configInputType++;
        if (configInputType > 2) configInputType = 0;
        
        if (configInputType == 1)      configCellCount = 2; 
        else if (configInputType == 2) configCellCount = 6; 
        else                           configCellCount = 0;
        beep(); 
      } 
      else if (activeMenuRow == 1) { 
        if (configInputType == 1) { 
          configCellCount = (configCellCount == 2) ? 3 : 2;
        } 
        else if (configInputType == 2) { 
          configCellCount++;
          if (configCellCount > 10) configCellCount = 6;
        }
        beep();
      }
      else if (activeMenuRow == 2) { 
        configServoRange = !configServoRange; 
        beep(); 
      } 
      else if (activeMenuRow == 3) { 
        configOutputCount = !configOutputCount;
        beep();
      }
      else if (activeMenuRow == 4) { 
        beep(); delay(60); beep();
        
        if (configServoRange == 0) {
          myServo1.setPulseRange(540, 2200);
          myServo2.setPulseRange(540, 2200);
        } else {
          myServo1.setPulseRange(500, 2500);
          myServo2.setPulseRange(500, 2500);
        }

        display.clearScreen(); 
        activeTestMenuRow = 0;
        currentSystemState = TEST_SELECTION_MENU;
        return;
      }
    }
    display.updateMenu(activeMenuRow, configInputType, configCellCount, configServoRange, configOutputCount);
  } 
  else if (currentSystemState == TEST_SELECTION_MENU)
  {
    if (delta != 0) {
      if (delta > 0) activeTestMenuRow++;
      else           activeTestMenuRow--;
      if (activeTestMenuRow > 3) activeTestMenuRow = 3;
      if (activeTestMenuRow < 0) activeTestMenuRow = 0;
    }

    if (encoderSwitch.isPressed()) {
      beep(); delay(60); beep();
      display.clearScreen();

      myServo1.init();
      if (configOutputCount == 1) {
        myServo2.init();
      }

      if (activeTestMenuRow == 0) {
        currentSystemState = MANUAL_POSITION;
        encoderValue = 50; 
      } 
      else if (activeTestMenuRow == 1) {
        currentSystemState = NEUTRAL_POSITION;
        encoderValue = 50; 
      } 
      else if (activeTestMenuRow == 2) {
        currentSystemState = VELOCITY_TEST;
        encoderValue = 25; 
        sweepValue = 0.0;
        sweepDirection = 1;
      }
      else if (activeTestMenuRow == 3) {
        currentSystemState = STAMPS;
        encoderValue = 50;
      }
      return;
    }
    display.updateTestSelectionMenu(activeTestMenuRow);
  }
  else 
  {
    static unsigned long buttonTimer = 0;
    static bool buttonActive = false;
    static bool longPressTriggered = false;

    if (digitalRead(ENCODER_SW_PIN) == LOW) {
      if (!buttonActive) {
        buttonActive = true;
        buttonTimer = millis();
        longPressTriggered = false;
      } 
      else if (!longPressTriggered && (millis() - buttonTimer > 400)) {
        longPressTriggered = true;
        
        currentSystemState = TEST_SELECTION_MENU;
        activeTestMenuRow = 0; 

        beep(); delay(60); beep(); 
        display.clearScreen();
        return; 
      }
    } 
    else {
      if (buttonActive) {
        buttonActive = false;
      }
    }

    int servoOutputValue = 50;

    if (currentSystemState == MANUAL_POSITION) 
    {
      if (delta != 0) encoderValue += (delta * ENCODER_STEP);
      if (encoderValue > 100) encoderValue = 100;
      if (encoderValue < 0)   encoderValue = 0;

      if (button1.isPressed()) { encoderValue = 0; beep(); }   // LEFT
      if (button2.isPressed()) { encoderValue = 50; beep(); }  // CENTER
      if (button3.isPressed()) { encoderValue = 100; beep(); } // RIGHT
      
      servoOutputValue = encoderValue;
    } 
    else if (currentSystemState == STAMPS)
    {
      if (delta != 0) encoderValue += (delta * ENCODER_STEP);
      if (encoderValue > 100) encoderValue = 100;
      if (encoderValue < 0)   encoderValue = 0;
      
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

      if (button1.isPressed()) { encoderValue = 15; beep(); }   // SLOW Sweep Speed (15%)
      if (button2.isPressed()) { encoderValue = 50; beep(); }   // MID Sweep Speed (50%)
      if (button3.isPressed()) { encoderValue = 100; beep(); }  // FAST Sweep Speed (100%)

      if (encoderValue > 0) {
        if (millis() - lastSweepTick >= 15) {
          lastSweepTick = millis();
          
          float stepSize = (float)encoderValue * 0.03;
          if (stepSize < 0.05) stepSize = 0.05; 

          sweepValue += ((float)sweepDirection * stepSize);
          
          if (sweepValue >= 100.0) { sweepValue = 100.0; sweepDirection = -1; }
          else if (sweepValue <= 0.0) { sweepValue = 0.0; sweepDirection = 1; }
        }
      }
      servoOutputValue = (int)sweepValue;
    }

    // MODIFIED: Restructured Button 3 handling logic to intercept presses during STAMPS mode
    if (button3.isPressed()) {
      if (currentSystemState == STAMPS) {
        // Toggle mode strictly between 0 and 1 to flip the visual header text state cleanly
        currentGlobalMode = (currentGlobalMode == 0) ? 1 : 0;
        beep();
      }
      else if (currentSystemState != MANUAL_POSITION && currentSystemState != VELOCITY_TEST) {
        // Run standard operating logic for non-excluded test modes
        currentGlobalMode++;
        if (currentGlobalMode > 2) currentGlobalMode = 0;
        myServo1.setMode(currentGlobalMode);
        if (configOutputCount == 1) myServo2.setMode(currentGlobalMode);
        beep(); delay(50); beep();
      }
    }

    myServo1.update(servoOutputValue);
    if (configOutputCount == 1) {
      myServo2.update(servoOutputValue);
    }

    display.updateTestScreen(encoderValue, currentGlobalMode, battery.getVoltage(), configServoRange, (int)currentSystemState);
    
    #if DEBUG_TELEMETRY
      printTelemetry();
    #endif
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
  #if DEBUG_TELEMETRY
    Serial.print("V: ");
    Serial.print(battery.getVoltage(), 2);
    Serial.print(" | S: ");
    Serial.print(currentSystemState);
    Serial.print(" | E: ");
    Serial.println(encoderValue);
  #endif
}
