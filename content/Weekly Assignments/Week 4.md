---
title: Laser Cutting
draft: false
tags:
  -
---
File Downloads:
 ![[lasercutting v14.f3d]]![[lasercutting.dxf]]
For this weeks assignment we had to create a laser cut model that can be assembled using only plug connections. The design was modelled in Fusion 360, prepared in Affinity Designer and cut on the larger CO2 laser cutter of the Fab-Lab.

![[Pasted image 20251103230342.png]]

First I created the complete design in Fusion without considering offsets. Here I used a parameter for the material width (3mm).

![[Pasted image 20251103230742.png]]
Afterwards I selected the different faces of the model and created another sketch based on them. I then created a parameter for the lasers kerf and used the offset tool to add the kerf to the new sketch. I also removed at least one line of the original sketch so that only the offset lines create a full face.

![[Pasted image 20251103231054.png]]

I then extruded the sketch with the added offset and created another sketch based on this to clean up all the construction lines. This sketch could then be exported as .dxf and imported into Affinity Designer.

![[Pasted image 20251103231224.png]]
In Affinity Designer I aligned all the different sketches to safe as much material as possible and check all of the vectors. I also added the Text for later engraving.