//
//  Fluid.cpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Fluid.hpp"

Fluid::Fluid(){
    radius = 0.1f;
    CreateSpheres();
    
}

void Fluid::CreateSpheres(){
    int gridSize = 8;
    float diff = 1 * radius;
    float y = 1.0f, z = -1.0f;
    for(int i=0; i<gridSize; i++){
        float z = -1.0f;
        for(int j=0; j<gridSize; j++){
            float x = -1.0f;
            for(int k=0; k<gridSize; k++){
                FluidParticle* newFlu = new FluidParticle(glm::vec3(x,y,z), radius);
                fluidParticles.push_back(newFlu);
                x+= diff;
            }
            z+= diff;
        }
        y -= diff;
    }
}


void Fluid::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->Draw(viewProjMtx,shader);
    }
}
