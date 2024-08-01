# **DIY Servo Tester for +180deg Servos!!!**

## **Table of Contents**
* [Summary](#summary)
* [BOM List](#bom-list)
* [Step-by-Step Building Guide](#step-by-step-building-guide)
* [User Guide](#user-guide)
* [Troubleshooting](#troubleshooting)

## **Summary**
This Project was created to expand the use of servo tester along with challenging my creation skills! This Tester not only checks the life of a servo but also expands the variety of servo types to servos with a larger range of motion. This tester includes features such as 3 turning modes(position control, neutral, speed control swipe) for everyone's needs! The main reason I design it, is because I had trouble finding a servo tester that met te expectations of mine ; 300deg servos and turn percentage preview. I hope this helps you too!

| ![CAD Image](/md_pictures/cad_full.png) | ![Physical Image](/md_pictures/physical_full.jpg) |
| --------------------------------------- | ------------------------------------------------- |

## **BOM List**
Here are the parts needed for this build!

* 1x Arduino Nano Type-C
* 1x I2C LCD Screen 16X2
* 2x Standard 16mm Pushbuttons
* 1x Switch
* 1x B10k Potentiometer
* 2x 220 Ohm Resistors
* 1x Jack Plug Input Connector
* 1x Jack Plug Power Supply 5V (I used 5V 3A)
* 6x Male Header Pins (3 in use, keep backup)
* 6x M3 6mm Screws
* 4x M3 8mm Screws
* 6x M3 Regular Nuts
* 5x Small Servo Screws
* 20-22 AWG Wire Gauge is preferred for the servo powering
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
* Use Supports (Case.stl, Lid.stl, Knob.stl)
* Don't Use Supports (Separator.stl, Seamer.stl)


| ![Case Part](/md_pictures/case_only.png)           | ![Lid Part](/md_pictures/lid_only.png)       |
| -------------------------------------------------- | -------------------------------------------- |
| Case.stl                                           | Lid.stl                                      |
| ![Separator Part](/md_pictures/separator_only.png) | ![Seamer Part](/md_pictures/seamer_only.png) |
| Separator.stl                                      | Seamer.stl                                   |
| ![Knob Part](/md_pictures/knob_only.png)           |                                              |
| Knob.stl                                           |

(Note: Use the orientation of the default stl file. These pictures are only demonstrative)

### Step 2: Cleaning the support material
Make sure that you remove carefully all of the support material from each part.
| ![Case Part](/md_pictures/steps/step02/image00032.jpeg) | ![Lid Part](/md_pictures/steps/step02/image00033.jpeg) |
| ------------------------------------------------------- | ------------------------------------------------------ |

### Step 3: Add User Interactive Parts
#### Jack Plug
Slide the Jack Plug in the slot shown in the pictures below. Use a ruler to align it with the outer surface. Use Super glue on around the plug to secure it in place
| ![Case Part](/md_pictures/steps/step03/i/image00030.jpeg) | ![Lid Part](/md_pictures/steps/step03//i/image00031.jpeg) |
| --------------------------------------------------------- | --------------------------------------------------------- |
#### Buttons
Then Slide the standard 16mm push buttons through the 2 middle holes of the Case.stl from top. Then fasten Them in place using the nuts that come in the package.\

| ![Case Part](/md_pictures/steps/step03/ii/image00029.jpeg) | ![Lid Part](/md_pictures/steps/step03//ii/image00028.jpeg) |
| ---------------------------------------------------------- | ---------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #31708f; background-color: #d9edf7; border-color: #bce8f1;">
Tip: Use a pair of pliers to tighten the nuts! 
</div>

#### Potentiometer 
Place the B10k Potentiometer from the inside of the Case.stl and fasten it with the nut & washer that come in the package

| ![Case Part](/md_pictures/steps/step03/iii/image00027.jpeg) | ![Lid Part](/md_pictures/steps/step03//iii/image00026.jpeg) |
| ----------------------------------------------------------- | ----------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #31708f; background-color: #d9edf7; border-color: #bce8f1;">
Tip: Use a pair of pliers to tighten the nut!
</div>

### Step 4: Add solder to pins

Add solder to all the pins of:
- Jack Plug
- Buttons
- Potentiometer


### Step 5: Prepare the LCD Screen
#### Remove the headers fom the lcd module
![Case Part](/md_pictures/steps/step05/i/image00024.jpeg)

#### Strip and Solder Cables
In his example I have used ribbon wire tap but feel free to use anything you want!
| ![Case Part](/md_pictures/steps/step05/ii/image00023.jpeg) | ![Case Part](/md_pictures/steps/step05/ii/image00022.jpeg) |
| ---------------------------------------------------------- | ---------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #8a6d3b;; background-color: #fcf8e3; border-color: #faebcc;">
Mark the GND wire with a marker! You wil thank me later. Of course colored cables are better.
</div>

#### Bend the wire like so
![Case Part](/md_pictures/steps/step05/iii/image00021.jpeg)

### Step 6: LCD Placement
Prepare cut servo screw to 7mm in length.
Finally we can place the LCD! Place the LCD like so and fasten it with the servo screws
| ![Case Part](/md_pictures/steps/step06/image00020.jpeg) | ![Case Part](/md_pictures/steps/step06/image00019.jpeg) |
| ------------------------------------------------------- | ------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #a94442; background-color: #f2dede; border-color: #ebccd1;">
Attention!!! Be careful of the screen's orientation. We don't want to flip the preview upside down!
</div>

### Step 7: Clean Separator.stl
Use a 3mm drill and redrill the 4 holes of the Separator.stl
![Case Part](/md_pictures/steps/step07/image00018.jpeg)

### Step 8: Separator Placement
Use 4 M3 8mm screws to secure the Separator.stl as shown below.
![Case Part](/md_pictures/steps/step08/image00017.jpeg)

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #a94442; background-color: #f2dede; border-color: #ebccd1;">
Attention!!! Don't forget to pass the cable through the slot!
</div>

### Step 9: Header Pins (Servo Output)
Get some male header pins and snap a triplet of. The sort side of the pins have to be from the inner side of the Case.stl. It's supposed to be press fit, but i suggest you use super glue for best security!

| ![Case Part](/md_pictures/steps/step09/image00016.jpeg) | ![Case Part](/md_pictures/steps/step09/image00015.jpeg) |
| ------------------------------------------------------- | ------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #31708f; background-color: #d9edf7; border-color: #bce8f1;">
Tip: Before glueing the pins in place connect a servo from the outer side of the Case.stl. In this way the pins are going to be perfectly aligned!
</div>

### Step 10: Solder to the Header Pins (Servo Output)
Add some solder to each of the 3 pins to prepare them for later.

![Case Part](/md_pictures/steps/step10/image00016.jpeg)

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #a94442; background-color: #f2dede; border-color: #ebccd1;">
Attention!!! Don't overheat the pins with your iron because they might dislocate!
</div>

### Step 11: Nano Placement
Cut a servo screw to 5mm in length
Slide the nano in the slot of the Case.stl. Slide it all the way and secure it with the servo screw along with the Seamer.stl. Use the picture for guidance!

![Case Part](/md_pictures/steps/step11/image00014.jpeg)

### Step 12: Push Button GND
Now we connect the GND to the push button. Use 12cm cable (around 26-28AWG) to connect it to one of the terminals of the pushbutton.

The push buttons are configured as INPUT_PULLUP. Chech more for INPUT_PULLUP [here](https://www.arduino.cc/reference/en/language/variables/constants/inputoutputpullup/)!

![Case Part](/md_pictures/steps/step10/image00016.jpeg)

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #31708f; background-color: #d9edf7; border-color: #bce8f1;">
<p>Info: If you have illuminated push-buttons instead of just connecting one of the terminals with a GND wire, you should bridge the LED (-) and one of the terminals with the GND wire, just like in my example!</p>

<p>I prefer the LED of the button to be always on (this tutorial follows that concept). But feel free to use any of the 2 scenarios in the schematic below!</p>

![LED Pushbutton Wiring](/hardware/led_pushbutton_wiring.webp)

(credits to reddit's "r/electrical" the post is [here]([text](https://www.reddit.com/r/electrical/comments/xo9q7q/led_button_wiring_help/?rdt=50037&onetap_auto=true&one_tap=true)))
</div>

### Step 13: Switch Connection
Connect the (+) terminal of the Jack Plug Connector to one of the terminals of the switch. Use 20-22 AWG Wire for the purpose

![Switch Wiring](/md_pictures/steps/step13/image00011.jpeg)


### Step 14: Header Pins Connection
get an old servo cable or make your own. Split the wires a bit and add solder to the ends. Connect the triplet to the headers and use some heatshrink if you like. Now split the triplet, power wires and signal wire, like shown. Run the power wires through the back and the signal towards the nano. Trim and solder the signal to A2 port on the nano.

| ![Switch Wiring](/md_pictures/steps/step14/i/image00010.jpeg) | ![Switch Wiring](/md_pictures/steps/step14/ii/image00009.jpeg) |
| ------------------------------------------------------------- | -------------------------------------------------------------- |
![Switch Wiring](/md_pictures/steps/step14/iii/image00008.jpeg)

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #a94442; background-color: #f2dede; border-color: #ebccd1;">
Attention!!! Don't solder the power wires just yet!
</div>

### Step 15: Nano Power Supply Connection
get an old servo triplet cable or make your own. Completely split the wires into the power cables (-, +) and the signal one (keep the signal for later). Connect the power cables in parallel with the ones from the header pins (heatshrink here!). Then connect the (+) to the NO terminal of the switch and the (-) to the (-) terminal of the jack plug. Then connect the nano power supply cable by connecting the source's (+) to Vin port and (-) to Any GND Port.

| ![Switch Wiring](/md_pictures/steps/step15/i/image00007.jpeg) | ![Switch Wiring](/md_pictures/steps/step15/ii/image00006.jpeg) |
| ------------------------------------------------------------- | -------------------------------------------------------------- |

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #a94442; background-color: #f2dede; border-color: #ebccd1;">
Attention!!! Use Heatshrink for this purpose to avoid any short-circuits
</div>

### Step 16: Pushbutton LED Positive Connection
Use a piece of wire around 10cm to connect the (+) terminals of the LEDs in the buttons. 

(This step is for those who have illuminated buttons)

### Step 17: Potentiometer Connections
Use the signal wire that you kept from step 15 to connect the middle terminal of the potentiometer. Use Heatshrink preferably. Trim the other side and connect it to A7 port on the nano.

![Switch Wiring](/md_pictures/steps/step17/i/image00003.jpeg)

The connect to the other terminals to pieces of 10cm wire for (-) and (+).

### Step 18: Powering Components
Merge all the (-) into one cable and all the (+) into another of the following components:
- Buttons (GND for sure, LED (+) if you have illuminated push-buttons)
- Potentiometer
- LCD Screen

Use headshrink!

<div style="padding: 15px; border: 1px solid transparent; border-color: transparent; margin-bottom: 20px; border-radius: 4px; color: #31708f; background-color: #d9edf7; border-color: #bce8f1;">
<p>Info: I usually get all the wires facing one direction, twist them together, then I control the wires like one and merge them with a single cable!</p>
</div>

### Step 19: Powering Cables to Nano
The 2 wires from step 15 (-, +) trim them and connect them to GND and 5V on the nano respectively

### Step 20: LCD Communication Wires
Connect the LCD's SDA cable to A4 port on the nano and the SCK to A5.

### Step 21: Push Button Wires
Connect the buttons output terminals to the nano:

* **M**ode Button → D3 Port
* **T**ype Button → D4 Port

### Step 22: Double Check Connections!
Now because I usually don't trust myself on these things i would use a multimeter and check connections again, verifying them with the schematic!

![Full Schematic](/hardware/electrical_schematic.png)

Note that in the schematic i am using regular buttons (w/o light). Step 16 explains the (+) connection of the LEDs. And step 12 about (-) (bridging).

### Step 23: Congrats! You finished all the Connections!
Now we are happy because all the difficult part is over!

**Always clean your mess after electronics!!!**

![Cable Mess](/md_pictures/steps/step23/image00001.jpeg)

### Step 24: Add nuts to the Lid.stl
Grab 6 M3 regular nuts and press fit them into place on the Lid.stl. If they are loose use a drop of super glue to better secure them!

![Adding Nuts to the Lid.stl](/md_pictures/steps/step24/image00002.jpeg)

### Step 25: Fasten the Lid.stl to Case.stl
Put the Lid to cover the electronic mess that you have done in the case. Use M3 6mm in length screws to fasten it in place!

![Fastening the Lid.stl to the Case.stl](/md_pictures/steps/step25/1.jpg)

### Step 26: Add the Knob.stl
Take the Knob.stl and press fit it on the Potentiometer.

**And that's it for the hardware!**

![Fastening the Lid.stl to the Case.stl](/md_pictures/steps/step26/1.jpg)

### Step 27: Install LCD's Library
For the program to run you need to install the LiquidCrystal_I2C.h library. Source files for the library [here](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library).

Credits to: fdebrabander and João Pedro São Gregorio Silva for the library

### Step 27: Upload program on the Nano

Get the code from [here](https://github.com/Agtheo09/diy-servo-tester/blob/main/software/main/main.ino) and upload it to the nano using the IDE of you choice! There are plenty of tutorial for how to do it. Make sure that you select the correct board type (Arduino Nano) and COM port!

### Step 28: Admiring the Beauty!
**Look at this......**
![Physical Image](/md_pictures/physical_full.jpg)


## User Guide

1. Connect 5V Power Supply (5V 3A works great for me!)
2. Flick the switch and wait
3. Press the T Button to start
4. Select the servo type by pressing the T button to switch the ranges from narrow to wide and the opposite
5. Select on of the Turning Modes by pressing the M Button (position control, neutral/middle, speed controlled swipe)
6. That is the most Easy in usage right?

## Troubleshooting

### Common Problems
- If the Screen is illuminated but doesn't show the chars check the SDA and SCK cables and connections. check the of the LCD and change it in the code if necessary on line 10. (Default is **0x27**)  
- With the buttons dont work check the ports of the connected Wires


I have a series of tests in [this folder](https://github.com/Agtheo09/diy-servo-tester/tree/main/software/tests). Use them to identify the problem! And feel free to reach me out on [printables](https://www.printables.com/@AggelosTheod_1409282)!

- button_test → Prints if the button on the port specified on line 1 in pressed or not in serial monitor
- lcd_test → Prints on Screen on the first line "Hello!" and of the Second "Servo Tester" (check the lcd address and change it if necessary on line 4)
- potentiometer_test → Prints the value of the potentiometer in serial. [0, 1024)