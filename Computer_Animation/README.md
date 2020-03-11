Remember to change the shader.vert and shader.frag path in Window.cpp if it's complaining about not find shader.

In Sphere CreateVert, inside for loop might need to equal to numLon/numLat


https://www.cs.cmu.edu/~scoros/cs15467-s16/lectures/11-fluids2.pdf
https://pdfs.semanticscholar.org/930a/9471033f3e2d177256f21c418616d14e90d5.pdf
http://www8.cs.umu.se/kurser/TDBD24/VT06/lectures/sphsurvivalkit.pdf
https://cg.informatik.uni-freiburg.de/publications/2014_EG_SPH_STAR.pdf



目前可公开情报:
    Rest Density会影响fluid particles 自然下落会不会爆出去
    StiffConstant会影响盒子里（外？ 小球之间的交错容忍程度以及活跃程度
