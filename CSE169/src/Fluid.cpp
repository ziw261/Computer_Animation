//
//  Fluid.cpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Fluid.hpp"

Fluid::Fluid(){
    radius = 0.5f;
    supportRadius = 1.1f;
    //boundCube = new Cube(glm::vec3(-30.0f,-30.0f,0.0f), glm::vec3(30.0f,30.0f,0.0f));
    CreateSpheres();
    //cerr << fluidParticles.size() << endl;
    //cerr << fluidParticles[0]->position.x << " " << fluidParticles[0]->position.y << " " << fluidParticles[0]->position.z << " " << endl;
     //cerr << fluidParticles[fluidParticles.size()-1]->position.x << " " << fluidParticles[fluidParticles.size()-1]->position.y << " " << fluidParticles[fluidParticles.size()-1]->position.z << " " << endl;
    
}

void Fluid::Update(float deltaTime){
    ZeroForce();
    
    /* Attention */
    ClearNeighbors();
    UpdateNeighbors();
    UpdateLocalDensities();
    
    ApplyGravity();
    
    /* Attention */
    UpdateParticleForces();
    
    UpdateParticles(deltaTime);
}


void Fluid::UpdateParticles(float deltaTime){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->Update(deltaTime);
    }
}

void Fluid::ZeroForce(){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->ZeroForce();
    }
}

void Fluid::CreateSpheres(){
    int gridSize = 8;
    float diff = radius;
    float y = 1.0f, z = -1.0f;
    for(int i=0; i<gridSize; i++){
        float z = -1.0f;
        for(int j=0; j<gridSize; j++){
            float x = -1.0f;
            for(int k=0; k<gridSize; k++){
                FluidParticle* newFlu = new FluidParticle(glm::vec3(x,y,z), radius,supportRadius);
                //cerr << newFlu->position.x << " " << newFlu->position.y << " " << newFlu->position.z << " " << endl;
                fluidParticles.push_back(newFlu);
                x+= diff;
            }
            z+= diff;
        }
        y -= diff;
    }
}


void Fluid::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    
    /* Attention */
    //boundCube -> draw(viewProjMtx, shader);
    
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->Draw(viewProjMtx,shader);
    }
}


void Fluid::ApplyGravity(){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->ApplyForce(gravity);
    }
}



/* Attention */
void Fluid::ClearNeighbors(){
    for (int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->neighbors.clear();
    }
}



/* Attention */
void Fluid::UpdateNeighbors(){
    for(int i=0; i<fluidParticles.size(); i++){
        for (int j=i+1; j<fluidParticles.size(); j++){
            float dist = glm::length(fluidParticles[i]->position - fluidParticles[j]->position);
            if(dist < supportRadius){
                fluidParticles[i]->neighbors.push_back(fluidParticles[j]);
                fluidParticles[j]->neighbors.push_back(fluidParticles[i]);
            }
        }
    }
}



/* Attention */
void Fluid::UpdateLocalDensities(){
    for(int i=0; i<fluidParticles.size();i++){
        fluidParticles[i]->UpdateLocalDensity();
    }
}



/* Attention */
void Fluid::UpdateParticleForces(){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->UpdateForces();
    }
}
