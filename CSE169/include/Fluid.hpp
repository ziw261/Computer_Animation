//
//  Fluid.hpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Fluid_hpp
#define Fluid_hpp

#include <stdio.h>
#include <iostream>
#include "FluidParticle.hpp"

using namespace std;

class Fluid{
public:
    // Member Variables
    vector<FluidParticle*> fluidParticles;
    float radius;
    
    // Member Functions
    Fluid();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void CreateSpheres();
    ~Fluid();
};


#endif /* Fluid_hpp */
