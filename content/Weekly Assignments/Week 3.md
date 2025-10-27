---
title: 3D Printing
draft: false
tags:
  -
---
File Downloads: 
![[ball-maze v4.f3d]]![[ball-maze v4.stl]]![[ball-maze v4.3mf]]
This weeks assignments was to print something that can only really be made using additive fabrication methods. My Idea for this was to create a small ball maze. It is constructed from concentric circles connected by wall segments. Because I only wanted to use 3D printing and didn't want to glue a see through plastic segment on top I let the walls of the ball maze grow out on the top and bottom so that they become so close that the ball wont fit through them.

![[Ball-Maze-3.png]]

I created my design in Fusion 360. For a more detailed Fusion walk through have a look at the previous assignment [[Week 2]]. The first thing for this design was a sketch with multiple circles to be extruded later.

![[Ball-Maze-4.png]]

After Extruding the walls of the maze, I created a second sketch and used the Offset tool in a new Sketch to offset the walls and create the thicker top and bottom part that the ball can't fit.

![[Ball-Maze-5.png]]

I then extruded the thicker part at the top and bottom of the walls of the maze and used the chamfer tool to make it possible to print the design without any supports.

![[Ball-Maze-6.png]]

After that i brought the design into Bambu Studio. Since I designed the Fusion Model with 3D Printing in mind, I could print the model immediately and to size.

![[Ball-Maze-7.png]] 

To print the Model I used my Bambulab A1 with a 0.4mm diameter nozzle and a textured PEI Plate. I then chose the "0.12mm Fine @BBL A1" Preset to allow for accurate printing of the model. This Profile uses a 0.12mm Layer height, which was sufficiently small to print this model. The only things I changed with this profile was the seam position and the brim. I set the seam position to random, since i wanted to print the small ball and I added an outer object brim of 5mm.

![[Seam.png]]
![[Bed-Adhesion.png]]

![[Ball-Maze1.png]]![[Ball-Maze2.png]]

The printed object looks very good, but sadly the ball maze design doesn't work too well. The reason for this is the flex of the maze, each ring is only connected to its outer ring with on wall, so there is enough wiggle room between the rings to allow the ball to fall out. Additionally I would use a metal ball next time since that would have more weight and make moving it in the labyrinth easier.