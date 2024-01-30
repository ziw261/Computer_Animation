Remember to change the shader.vert and shader.frag path at line 60 in Window.cpp if it's complaining about not find shader.


To run different project, remember to change the line 78 in Window.cpp to load differnt project. 
Options: loadFluid, loadAnimation, loadCloth, loadSkeleton, loadSkin

When loading Fluid:
Use WASD to resize the container. Use q to release the container.

When loading Cloth:
Use C V to control the strongness of wind. Press T to release the cloth.
Use WASD to move the cloth

When loading Skeleton:
Use Up and Down to zoom in and out

When loading Skin:
Press X Y Z  and LEFT RIGHT key to first choose which degree of freedom to change.
Then press UP and DOWN to change the value.

When loading Animation:
Press P to pause.

Reference for SPH algorithm in Fluid Simulation:
https://www.cs.cmu.edu/~scoros/cs15467-s16/lectures/11-fluids2.pdf
https://pdfs.semanticscholar.org/930a/9471033f3e2d177256f21c418616d14e90d5.pdf
http://www8.cs.umu.se/kurser/TDBD24/VT06/lectures/sphsurvivalkit.pdf
https://cg.informatik.uni-freiburg.de/publications/2014_EG_SPH_STAR.pdf


