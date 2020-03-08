//
//  FluidParticle.cpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "FluidParticle.hpp"

FluidParticle::FluidParticle(glm::vec3 position, float radius){
    this->radius = radius;
    this->position = position;
    sphere = new Sphere(radius);
    
}



void FluidParticle::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    sphere->toWorld = glm::translate(glm::mat4(1.0f), position);
    sphere->Draw(viewProjMtx, shader);
}
