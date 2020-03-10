//
//  FluidParticle.cpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "FluidParticle.hpp"
float PI = glm::pi<float>();

FluidParticle::FluidParticle(glm::vec3 position, float radius, float supportRadius){
    this->mass = 1.0f;
    this->velocity = glm::vec3(0,0,0);
    this->force = glm::vec3(0,0,0);
    this->radius = radius;
    this->position = position;
    sphere = new Sphere(radius);
    this->smoothingLength = supportRadius;
    localDensity = pressure = 0.0f;
    viscosity = 0.1f;
    
}



void FluidParticle::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    sphere->toWorld = glm::translate(glm::mat4(1.0f), position);
    sphere->Draw(viewProjMtx, shader);
}


void FluidParticle::UpdateForces(){
    ApplyPressureForce();
    ApplyViscosityForce();
}



void FluidParticle::Update(float deltaTime){
    //cerr << force.x << " " << force.y << " " << force.z << " " << endl;
    glm::vec3 acceleration = force/mass;
    //glm::vec3 acceleration = force/localDensity;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    float deCoeff = -1.0f;
    
    if(position.y < -10){
        reBounceCounter++;
        if(reBounceCounter > 7){
            deCoeff = -0.9f;
        }
        position.y  = 2* (-10) -  position.y;
        velocity.y = deCoeff * velocity.y;
        velocity.x = (1-0.2) * velocity.x;
        velocity.z = (1-0.2) * velocity.z;
        //cerr << "been here" << endl;
    }

}

void FluidParticle::UpdatePressure(){
    pressure = stiffConst * (localDensity - restDensity);
    //cerr << localDensity << endl;
}


void FluidParticle::ApplyPressureForce() {
    UpdatePressure();
    glm::vec3 fpressure = glm::vec3(0);
    for(int i=0; i<neighbors.size(); i++){
        glm::vec3 r = position - neighbors[i]->position;
        glm::vec3 wspikyGradient = CalWSpikyGradient(r, smoothingLength);
        fpressure  += (neighbors[i]->mass / neighbors[i]->localDensity) * ((pressure + neighbors[i]->pressure) / 2.0f) * wspikyGradient;
    }
    ApplyForce(-1.0f * fpressure);
     
     
}

/* Attention */
void FluidParticle::ApplyViscosityForce() {

    glm::vec3 fviscosity = glm::vec3(0);
    
    for(int i=0; i<neighbors.size(); i++){
        glm::vec3 r = position - neighbors[i]->position;

        float wviscosityLaplacian = CalWViscosityLaplacian(r, smoothingLength);
        fviscosity += viscosity * (neighbors[i]->mass / neighbors[i]->localDensity) * (neighbors[i]->velocity - velocity) * wviscosityLaplacian;
    }
    ApplyForce(fviscosity);
     
}

void FluidParticle::UpdateLocalDensity() {
    int size = neighbors.size();
    float temp = 0;
    for (int i = 0; i < size; i++)
    {
        glm::vec3 r = position - neighbors[i]->position;
        float w = CalWPoly(r, smoothingLength);
        temp += neighbors[i]->mass * w;
    }
    localDensity = temp;
    
}

float FluidParticle::CalWPoly(glm::vec3 r, float h){
    float r_norm = glm::length(r);
    float result = 0;
    
    if(r_norm <= h) {
        result = (315.f/(64.f * PI * glm::pow(h,9))) * glm::pow(glm::pow(h,2) - glm::pow(r_norm,2),3);
    }
    
    return result;
}


glm::vec3 FluidParticle::CalWSpikyGradient(glm::vec3 r, float h){
    float r_norm = glm::length(r);
    glm::vec3 result;
    if(r_norm<=h && r_norm > glm::epsilon<float>()) {
        result = ((- 45.f * (float)glm::pow(h-r_norm,2)) / (r_norm * PI * (float)glm::pow(h,6))) * r;
    }
    return result;
}


float FluidParticle::CalWViscosityLaplacian(glm::vec3 r, float h){
    float r_norm = glm::length(r);
    float result = 0;
    if(r_norm<=h && r_norm > glm::epsilon<float>()) {
        result = 45.f / (PI * (float)glm::pow(h,6)) * (h - r_norm);
    }
    return result;
}



