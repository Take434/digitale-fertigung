---
title: Final Project - Flipping Dots
draft: false
tags:
  -
---
# Poster

# Presentation
This is the presentation I held in front of the class to explain my project to everyone.
Everything in it will be explained on this page, it is only included for completeness.
![[finalProjectPresentation.pdf]]
# Artifacts
Controller Board KiCad Project (also includes schematic for decoder):
![[content/files/finalProject/digiFabFinal.zip|digiFabFinal]]
Decoder Board KiCad Project:
![[digiFabFinal-decoder.zip]]
Spacer Blocks Fusion Archive:
![[flipdot-spacers.f3d]]
Case Stand Fusion Archive:
![[flipdot-stand.f3d]]
Case design for laser cutting:
![[box-all-correct.dxf]]
# The Idea (again)
This is just a quick reiteration of my final project Idea. The full description can be found in [[Final Project Idea]].

For my final project I wanted to turn an old Flipdot module I had bought off of Ebay into a functional display. This module was produced by the company BROSE and used mostly as a display in busses. Nowadays Flipdot modules are not really produced anymore.

My Idea was to control this display using my own controller board. Then I wanted to include a joystick and make it possible to play snake on the display.
# Understanding the BROSE Flipdot Module
The first step was understanding the Flipdot module. This was problematic since I could not find any official documentation online. The only thing I managed to find was [this blog](https://radow.org/flip-dot.php) by Rainer Radow, documenting the functionality of the module. While it helped me tremendously it was not really exhaustive and I still had to do a lot of testing to figure out how to address my module.
## How Does the Module work?
So, in order to understand how the Flipdot module works, we need to understand a single dot first.
<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Close Up Pic of a single dot and the coils beneath</div>
</div>
This is a single dot, it is made up of three things:
1. A magnetic top plate, black on one side yellow on the other
2. Two coils with an iron core forming an electro magnet
3. Two connections to the underlying board
The dot is either yellow or black and stays that way because of the polarity of the electro magnet beneath. In order to flip the dot, the two connections have to be powered to reverse the magnets polarity. It is important to only give a quick impulse to the dot otherwise the coils might be damaged.

The module is made up of 448 dots, arranged into a 16 x 28 matrix.
The individual dots are addressed via rows and columns:
![[Pasted image 20260216123038.png]]
	_Image from: https://radow.org/flip-dot.php, all rights belong to Rainer Radow_

Here you can see how four dots (`L1` to `L4`) are addressed in the matrix. Every column (`X_SPALTE`) has one connection and every row (`Y_ZEILE`) has two. Each dot has two diodes making sure the signal flows from row to column and not between the two row connections.
Two flip a dot you have to connect one of the rows and the column. This way the polarity of the signal can be switched depending on the connected row and the polarity of the column.

<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Testing one specific dot with my lab supply</div>
</div>
This is me testing one specific dot using my lab power supply to control the voltage. At least 9 Volts are needed for the dot to flip, 12V produce a good, fast flip. According to Radow, 24V were used in busses and such.
## How to Address the Module
Now that we understand the basic functionality of the module we need to discuss how to address it properly. Beside the matrix itself the module also the following other components:
- A 60pin Connector
- Comparator logic for module selection
- A Chip to drive the columns
These components need to be understood to address the module correctly. [Radow's blog](https://radow.org/flip-dot.php) helped a lot with this, but I also needed to test many of these components myself to actually understand them.
### The Comparator Logic
<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Image of the comparator set to 1 off everything else on</div>
</div>
The Comparator can be neglected in this case. It was used to determine which module should be addressed when they were placed in series. In my case the first switch needs to be off and everything else should be on (according to Radow's blog and my testing).
### The FP2840 Chip
<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Image of the Chip placed on the Display and the pinout next to it (photoshopped)</div>
</div>
This chip controls all 28 columns of the display. It is a "one of 28" decoder and was used to drive 7-segment displays. Depending on 6 Inputs It provides a positiv or negativ output on one of the 28 output pins. The truth table can be found in the datasheet: [[fp2800-datasheet.pdf]].

According to Radow's blog, this chip is at risk if the electrical impulse is not timed correctly since it switches the actual 12V driving the dots. Because of this I elected to test the board without the chip, rather switching with jumper cables instead.
![[IMG_3589.png]]
### The 60pin Connector
<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Image of the 60pin connector (on board)</div>
</div>
This connector is what I have to actually address to drive the display. Most BROSE modules have two, one input and one output to drive them in series. If there are two, the left one is the input connector.

Most of the pins on this connector are used to address the rows of the display directly. There are 40 total connections for 20 possible rows since each row has one negativ and one positiv connection (as was explained above [[#How Does the Module work?]]). My module only has 16 rows so 8 pins are actually not connected. 
![[Pasted image 20260216132740.png]]
	_Image from: https://radow.org/flip-dot.php, all rights belong to Rainer Radow_

The other pins are used to address the chip and the comparator logic.
# Actually Flipping Dots (Testing)
To actually flip a single dot (while not using the chip), you need to do the following:

|                      | 60pin Connector                                  | Chip                 |
| -------------------- | ------------------------------------------------ | -------------------- |
| From black to yellow | 12V connected to SET-X<br>GND connected to GND   | GND connected to row |
| From yellow to black | GND connected to RESET-X<br>12V connected to 12V | VS connected to row  |
But how could I achieve this with a microcontroller? There needed to be something to switch the 12 Volts and GND. For the initial tests Ahmed provided me with a Motor Diver IC that was basically a double H-Bridge:
![[IMG_3898.png]]
Testing with an Arduino, I first drew this schematic. The Arduino addresses the motor driver via four digital pins and a button is connected to another digital pin. The 12V power supply is the lab supply from the FabLab. This is then connected to the 60pin Connector of the Flipdot display and the chip is replaced with a jumper wire:
![[IMG_3600.png]]
The Arduino can then detect a button press and configure the motor driver to output 12V / GND depending on the state of the dot. The only manual input necessary is switching the connection of the jumper wires replacing the chip.

Since I this setup using the motor driver only let me control two rows maximum I elected to test using MOSFETs.![[IMG_3619.png]]
The Lab only had NPN-MOSFETs meaning that I could only connect GND to a specific pin. Because of this I set my test up to connect GND to the Reset pin when I press the button. This allowed me to flip a dot from yellow to black on button press. It also proved that I could use MOSFETs to address the Reset pins. I would only need PNP-MOSFETs to address the Set pins and everything should work out.
# Designing the Boards
Now that I have tested the Flipdot module with a MOSFET it is time to design my own board.
I have decided to split my design into two, one board for the logic and one to address the rows of my display. This second board needs to have 32 MOSFETs and after consulting with Ahmed I decided to order it off of JLCPCB already fully assembled to save time.
## Controller Board
![[Pasted image 20260216144105.png]]
The controller board is going to do the computation and handle the logic of the whole project. It needs to connect to three external components:
- A Computer via the Serial to USB interface we built in [[Week 7]]
- The Joystick
- The Decoder Board

Another thing the controller board needs to handle is power. Since the dots need 12V to flip, the board uses a linear voltage regulator to step 12V down to 5V. This means that the 12V can be passed to the decoder board and the controller can use the stepped down 5V eliminating the necessity for the lab power supply.

The chip we are using for this class is the ATmega328P-A which has 9 digital pins. But my board needs more than that:

| Pin Count | Usage                      |
| --------- | -------------------------- |
| 1         | Joystick Button            |
| 6         | Custom Decoder Board       |
| 6         | Chip on the Flipdot module |
Because of this I elected to use a GPIO expander. This IC is addressed via I2C and provides 16 additional digital Pins.

The full bill of material for this board can be found here (exported from KiCad Project): ![[digiFabFinal.csv]]
![[Pasted image 20260216145107.png]]
The routing for this board was pretty challenging, because of this I chose to use two layers, the front and the back of the board. This meant I had to use VIAs, but more on that in [[#Manufacturing the Controller Board]]. But because of that I managed to fit all of my components very close together avoiding wasting too much material.

The full KiCad Project for this Board is linked above, in [[#Artifacts]].
## Decoder Board
My decoder board needs to do one thing: address the 32 pins for the rows of my display.
I need to be able to have all the Reset pins unconnected or high and all the Set pins unconnected or GND. Then I have to be able to pull one specific Reset pin down to GND or one specific Set pin up to 12V.

![[IMG_3616.png]]
At first I decided I should create the board using six Multiplexers. This would allow me to address 32 pins and provide them with either GND or 12V. But with this design it is not possible to have multiple outputs high at the same time, which would be necessary for the Reset pins.

![[Pasted image 20251209151758.png]]
Then I found this graphic explaining how to create a 5 to 32 line decoder and decided that I would need to implement this logic if I wanted to address my display. This allows me to address all 32 pins individually and only uses 5 digital pins of the ATmega. I would need to have a MOSFET behind each output of the decoder but that is not a problem.

![[Pasted image 20260216145827.png]]
This is the design I came up with according to the diagram above. I am using five decoder ICs. Behind each output is a MOSFET, 16 are P-Channel and 16 are N-Channel. This allows me to address all 32 pins of the Flipdot display.

![[IMG_3801.png]]
This is the resulting PCB, I also used two layers to make routing simpler and then ordered it off JLCPCB.

The full KiCad Project for this Board is linked above, in [[#Artifacts]].
# Manufacturing the Controller Board


# The Joystick and Case

# The Code

# Problems with the Decoder Board, Changes for v2

# Problems with v2, Changes for v3

