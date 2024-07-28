# **DIY Servo Tester for +180deg Servos!!!**

## **Summary**

This Project was created to expand the use of servo tester along with challenging my creation skills! This Tester not only checks the life of a servo but also expands the variety of servo types to servos with a larger range of motion. This tester includes features such as 3 turning modes(position control, neutral, speed control swipe) for everyone's needs! The main reason I design it, is because I had trouble finding a servo tester that met te expectations of mine ; 300deg servos and turn percentage preview. I hope this helps you too!

!!!PICTURES HERE!!!

## **Table of Contents**
* [BOM List](#bom-list)
* [Step-by-Step Building Guide](#step-by-step-building-guide)

## **BOM List**
Here are the parts needed for this build!

* 1x Arduino Nano
* 1x I2C LCD Screen 16X2
* 2x Standard 16mm Pushbuttons
* 1x Switch
* 1x B10k Potensiometer
* 1x Jack Plug Input Connector
* 6x Male Header Pins (3 in use, keep backup)
* 6x M3 6mm Screws
* 4x M3 8mm Screws
* 6x M3 Regular Nuts
* 5x Small Servo Screws
* 22-24 AWG Wire Gauge is preferred for the servo powering
* 26-30 AWG Wire Gauge is preferred for Arduino Signals (buttons potentiometers etc)
  
Of course you will need a 3D Printer and some Nice Filament to print this beauty!

## **Step-by-Step Building Guide**

### Step 1: Printing All the Parts!
I used my Original Prusa MK4 with ESun PLA+ for this example.

Here is my preference in Printing Settings:
* Nozzle: 0.4mm
* Layer Height: 0.2mm
* Infill: 60%
* Perimeters: 3
* No brim or Raft
* Use Supports (Case.stl, Lid.stl)
* Don't Use Supports (Separator.stl, Seamer.stl)


| ![Case Part](/md_pictures/case_only.png)           | ![Lid Part](/md_pictures/lid_only.png)       |
| -------------------------------------------------- | -------------------------------------------- |
| Case.stl                                           | Lid.stl                                      |
| ![Separator Part](/md_pictures/separator_only.png) | ![Seamer Part](/md_pictures/seamer_only.png) |
| Separator.stl                                      | Seamer.stl                                   |

(Note: Use the orientation of the default stl file. These pictures are only demonstrative)

### Step 2: Cleaning the support material
Make sure that you remove carefully all of the support material from each part.

### Step 3: Add User Interactive Parts

### Step 4: Add LCD Screen