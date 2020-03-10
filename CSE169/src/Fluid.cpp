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
    supportRadius = 1.5f;
    CreateSpheres();
    //cerr << fluidParticles.size() << endl;
    //cerr << fluidParticles[0]->position.x << " " << fluidParticles[0]->position.y << " " << fluidParticles[0]->position.z << " " << endl;
     //cerr << fluidParticles[fluidParticles.size()-1]->position.x << " " << fluidParticles[fluidParticles.size()-1]->position.y << " " << fluidParticles[fluidParticles.size()-1]->position.z << " " << endl;
    
}

void Fluid::Update(float deltaTime){
    ZeroForce();
    ClearNeighbors();
    UpdateNeighbors();
    UpdateLocalDensities();
    ApplyGravity();
    UpdateParticleForces();
    ApplyBound();
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
    gridSize = 10;
    //float diff = 2*radius;
    float diff = radius;
    float y = 5.0f;
    for(int i=0; i<gridSize; i++){
        float z = -1.0f;
        for(int j=0; j<gridSize; j++){
            float x = -1.0f;
            for(int k=0; k<gridSize; k++){
                FluidParticle* newFlu = new FluidParticle(glm::vec3(x,y,z), radius,supportRadius);
                //cerr << newFlu->position.x << " " << newFlu->position.y << " " << newFlu->position.z << " " << endl;
                fluidParticles.push_back(newFlu);
                x+= diff;
                //cerr << x << endl;
            }
            z+= diff;
        }
        y -= diff;
    }
    
    maxXZ = gridSize/2.f - 0.5f - .5f + .5f;
    //maxXZ = 9.f;
    minXZ = -1.5f;
    minY  = 5 - (gridSize/2 -0.5f) - .1f;
    //minY = -4.1f;
    sideHeight = minY;
    //cerr << fluidParticles.size() << endl;
    //cerr << maxXZ << " " << minXZ << " " << minY << endl;
    //cerr << fluidParticles[0]->position.x << " " << fluidParticles[0]->position.y << " " << fluidParticles[0]->position.z << " " << endl;
    //cerr << fluidParticles[fluidParticles.size()-1]->position.x << " " << fluidParticles[fluidParticles.size()-1]->position.y << " " << fluidParticles[fluidParticles.size()-1]->position.z << " " << endl;
}



void Fluid::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->Draw(viewProjMtx,shader);
    }
}


void Fluid::ApplyGravity(){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->ApplyForce(gravity);
    }
}


void Fluid::ClearNeighbors(){
    for (int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->neighbors.clear();
    }
}


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


void Fluid::UpdateLocalDensities(){
    for(int i=0; i<fluidParticles.size();i++){
        fluidParticles[i]->UpdateLocalDensity();
    }
}


void Fluid::UpdateParticleForces(){
    for(int i=0; i<fluidParticles.size(); i++){
        fluidParticles[i]->UpdateForces();
    }
}


void Fluid::ApplyBound(){
    // x,z: -1 -> 2.5
    // y    -0.5 -> 3

    if(!shouldBeBound){
        minY = -10.0f;
    }
        //minY  = -0.9f;
    for(int i=0; i<fluidParticles.size(); i++){
        if(fluidParticles[i]->isFreed){
            continue;
        }
        if(!shouldBeBound && fluidParticles[i]->position.y < sideHeight - 7*radius){
            //cerr << sideHeight - 20*radius << endl;;
            fluidParticles[i]->isFreed = true;
            continue;
        }
        if(fluidParticles[i]->position.x + radius >= maxXZ){
            glm::vec3 e = glm::vec3(1.0f,0.0f,0.0f);
            float l = 0.5f - fluidParticles[i]->position.x;
            float fsd = -1.0f * springConst * (radius - l) - dampFact * (glm::dot(e, fluidParticles[i]->velocity));
            glm::vec3 f1 = fsd * e;
            fluidParticles[i]->ApplyForce(f1);
        }
        if(fluidParticles[i]->position.x + radius <= minXZ){
            glm::vec3 e = glm::vec3(-1.0f,0.0f,0.0f);
            float l = 0.5f + fluidParticles[i]->position.x;
            float fsd = -1.0f * springConst * (radius - l) - dampFact * (glm::dot(e, fluidParticles[i]->velocity));
            glm::vec3 f1 = fsd * e;
            fluidParticles[i]->ApplyForce(f1);
        }
        if(fluidParticles[i]->position.z + radius <= minXZ){
            glm::vec3 e = glm::vec3(0.0f,0.0f,-1.0f);
            float l = 0.5f + fluidParticles[i]->position.z;
            float fsd = -1.0f * springConst * (radius - l) - dampFact * (glm::dot(e, fluidParticles[i]->velocity));
            glm::vec3 f1 = fsd * e;
            fluidParticles[i]->ApplyForce(f1);
        }
        if(fluidParticles[i]->position.z + radius >= maxXZ){
            glm::vec3 e = glm::vec3(0.0f,0.0f,1.0f);
            float l = 0.5f - fluidParticles[i]->position.z;
            float fsd = -1.0f * springConst * (radius - l) - dampFact * (glm::dot(e, fluidParticles[i]->velocity));
            glm::vec3 f1 = fsd * e;
            fluidParticles[i]->ApplyForce(f1);
        }
        if(fluidParticles[i]->position.y + radius <= minY){
            glm::vec3 e = glm::vec3(0.0f,-1.0f,0.0f);
            float l = 0.5f + fluidParticles[i]->position.y;
            float fsd = -1.0f * springConst * (radius - l) - dampFact * (glm::dot(e, fluidParticles[i]->velocity));
            glm::vec3 f1 = fsd * e;
            fluidParticles[i]->ApplyForce(f1);
        }
    }
}
