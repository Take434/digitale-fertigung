---
title: Final project idea
draft: false
tags:
  -
---
For my final Project I would like to create something using an old Filp-Dot display which I already have. The goal is to mount the display to the wall, preferably in a nice wooden frame and have it display something. The complexity of the project varies greatly depending on the things I would like to display, so I will start with the time and date.
 ![[files/Flip-Dot.png]]
 ![[content/files/finalProjectSketch/39.png]]
## Components of the Project
This is my first idea of the things I need to built for this project.

**The Frame**  
I want to build a nice wooden frame to hold the display and assorted electronics. It has to be wall mountable and optionally include a foot to stand on its own.

**The Display**  
As mentioned above, I already have an old Flip-Dot display. But I have yet to figure out how to control the display so I need to map out the different pins and connections of the display to figure out how to control it.

**The Circuit**  
After I have figured out how to control the display, I need to create my own PCB, including a microcontroller, that plugs into the back of the display and can control it. The PCB also needs to handle power, since I would like to have only one plug to connect the finished piece to a wall outlet. This power needs to be converted to the correct levels

## Further Ideas
Here are some Ideas what to create with the display besides a clock:
- Calendar - connect the Display to my google calendar and display events throughout the day
- Picture Frame - Display different Pixel art images and animations on the display. You could have a database in the background that stores the images and switch through them every day
- Games - Connect a controller and play: snake, tetris, pong, space defenders
- Countdown - configure important events or todos and display a count down (in days) of the time remaining
- Arbitrary Messages - Display Text messages that can be entered via a webapp
- Multiple Views - Create a system to flip through multiple views on the display (maybe via buttons, maybe view a mobile app) the views could be any of the above