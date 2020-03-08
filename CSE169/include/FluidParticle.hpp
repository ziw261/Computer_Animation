//
//  FluidParticle.hpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef FluidParticle_hpp
#define FluidParticle_hpp

#include <stdio.h>
#include "core.h"
#include "Particle.hpp"
#include "Sphere.hpp"
#include <iostream>

class FluidParticle : public Particle{
public:
    
    // Member Variable
    float radius;
    Sphere* sphere;
    vector<FluidParticle* > neighbors;
    
    
    
    // Member Function
    FluidParticle(glm::vec3 position, float radius);
    void Update();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void ZeroForce();
    void ApplyGravity();

    ~FluidParticle();
    
};


#endif /* FluidParticle_hpp */
