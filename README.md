# **DIY Servo Tester for +180deg Servos!!!**

## **Summary**

This Project was created to expand the use of servo tester along with challenging my creation skills! This Tester not only checks the life of a servo but also expands the variety of servo types to servos with a larger range of motion. This tester includes features such as 3 turning modes(position control, neutral, speed control swipe) for everyone's needs! The main reason I design it, is because I had trouble finding a servo tester that met te expectations of mine ; 300deg servos and turn percentage preview. I hope this helps you too!

| ![CAD Image](/md_pictures/cad_full.png) | ![Physical Image](/md_pictures/physical_full.jpg) |
| --------------------------------------- | ------------------------------------------------- |

## **Table of Contents**
* [BOM List](#bom-list)
* [Step-by-Step Building Guide](#step-by-step-building-guide)

## **BOM List**
Here are the parts needed for this build!

* 1x Arduino Nano Type-C
* 1x I2C LCD Screen 16X2
* 2x Standard 16mm Pushbuttons
* 1x Switch
* 1x B10k Potentiometer
* 2x 220 Ohm Resistors
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

# Under Construction...