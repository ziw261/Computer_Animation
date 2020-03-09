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


/* Attention */
void FluidParticle::UpdateForces(){
    ApplyPressureForce();
    ApplyViscosityForce();
    //ApplyRepulsion();
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
        if(reBounceCounter > 15){
            deCoeff = -0.9f;
        }
        position.y  = 2* (-10) -  position.y;
        velocity.y = deCoeff * velocity.y;
        velocity.x = (1-0.2) * velocity.x;
        velocity.z = (1-0.2) * velocity.z;
        //cerr << "been here" << endl;
    }
     
     
     
     
    
}


/* Attention */
void FluidParticle::UpdatePressure(){
    pressure = stiffConst * (localDensity - restDensity);
    //cerr << pressure << endl;
}


/* Attention*/
glm::vec3 FluidParticle::GetSpikyKernelGradient(float q, glm::vec3 posDiff)
{
    if (q >= 0 && q <= 1)
    {
        glm::vec3 leftOp = (-24.0f  * posDiff)/ (PI * (float)pow(smoothingLength, 8));
        
        float rightOp = pow(pow(smoothingLength, 2) - pow(glm::length(posDiff), 2), 2);
        
        glm::vec3 res = (leftOp * rightOp);
        
        if (glm::length(res) > 0.001f)
        {
           
            res = glm::normalize(res) * 0.001f;
        }
        
        return res;
    }
    
    return glm::vec3(0);
}


/* Attention */
void FluidParticle::ApplyPressureForce() {
    UpdatePressure();
    
    /*
    glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
    int size = neighbors.size();
    for (int i = 0; i < size; i++)
    {
        float leftOp = (pressure + neighbors[i]->pressure) / (2 * neighbors[i]->localDensity);
        float q = glm::length(position - neighbors[i]->position) / smoothingLength;
        tot = tot + (leftOp * GetSpikyKernelGradient(q, position - neighbors[i]->position));
        
    }
    glm::vec3 res = (-1.0f * mass * tot);
    ApplyForce(res);
     */
     
    
    
    glm::vec3 fpressure = glm::vec3(0);
    for(int i=0; i<neighbors.size(); i++){
        glm::vec3 r = position - neighbors[i]->position;
        glm::vec3 wspikyGradient = CalWSpikyGradient(r, smoothingLength);
        fpressure  += (neighbors[i]->mass / neighbors[i]->localDensity) * ((pressure + neighbors[i]->pressure) / 2.0f) * wspikyGradient;
    }
    ApplyForce(-1.0f * fpressure);
     
     
}


/* Attention */
void FluidParticle::ApplyRepulsion()
{
    int collisionType = HandleCollision();
    if (collisionType != 0)
    {
       
        HandleImpulse(collisionType);
    }
}


/* Attention */
int FluidParticle::HandleCollision()
{
    float bound = 10.0f;
    bool hit = 0;
    glm::vec3 collisionNormal;
    //The left and right sides
    if (position.x < (-1.0f * bound) + radius)
    {
        collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
        return 1;
    }
    if (position.x >(bound - radius))
    {
        collisionNormal = glm::vec3(1.0f, 0.0f, 0.0f);
        return 1;
    }
    //The top and bottom sides
    if (position.y < (-1.0f * bound) + radius)
    {
        //cout << " IN THE RIGHT CASE " << endl;
        collisionNormal = glm::vec3(0.0f, -1.0f, 0.0f);
        return 2;
    }
    if (position.y > (bound - radius))
    {
        collisionNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        return 2;
    }
    // The front and back sides
    if (position.z < (-1.0f * bound) + radius)
    {
        collisionNormal = glm::vec3(0.0f, 0.0f, -1.0f);
        return 3;
    }
    if (position.z >(bound - radius))
    {
        collisionNormal = glm::vec3(0.0f, 0.0f, 1.0f);
        return 3;
    }
    float collisionDepth = glm::dot(position, collisionNormal) - (bound - radius);
    /* First apply repulsion force */
    glm::vec3 resForce = (-1.0f * springConst * collisionDepth * collisionNormal);
    /* Then apply the damping force */
    resForce = resForce + (dampFact * collisionNormal);
    //applyForce(resForce);
    return hit;
}


/* Attention */
void FluidParticle::HandleImpulse(int collisionType)
{
    if (collisionType == 1)
    {
        velocity.x = (velocity.x * -.5f);
    }
    if (collisionType == 2)
    {
        velocity.y = (velocity.y * -1.0f);
    }
    if (collisionType == 3)
    {
        velocity.z = (velocity.z * -.5f);
    }
}


/* Attention */
float FluidParticle::GetViscousLagrangian(float q)
{
    if (q >= 0 && q <= 1)
    {
        float leftOp = 40.0f / (PI * pow(smoothingLength, 4));
        float res = leftOp * (1 - q);
       
        return res;
    }
    
    return 0.0f;
}



/* Attention */
void FluidParticle::ApplyViscosityForce() {
    //glm::vec3 viscousForce = mass * viscosity * getViscosityGradient();
    
    /*
    glm::vec3 tot = glm::vec3(0.0f, 0.0f, 0.0f);
    int size = neighbors.size();
    for (int i = 0; i < size; i++)
    {
        glm::vec3 velDiff = neighbors[i]->velocity - velocity;
        float q = glm::length(position - neighbors[i]->position) / smoothingLength;
        tot = tot + ((velDiff / neighbors[i]->localDensity) * GetViscousLagrangian(q));

    }
    glm::vec3 res = viscosity * mass * tot;

    ApplyForce(viscosity * mass * tot);
    */
    
    
    
    glm::vec3 fviscosity = glm::vec3(0);
    
    for(int i=0; i<neighbors.size(); i++){
        glm::vec3 r = position - neighbors[i]->position;

        float wviscosityLaplacian = CalWViscosityLaplacian(r, smoothingLength);
        fviscosity += viscosity * (neighbors[i]->mass / neighbors[i]->localDensity) * (neighbors[i]->velocity - velocity) * wviscosityLaplacian;
    }
    ApplyForce(fviscosity);
     
}



/* Attention */
float FluidParticle::Kernel(glm::vec3 nPosition, glm::vec3 locPosition)
{
    float q = glm::length(nPosition - locPosition) / smoothingLength;
    if (q >= 0 && q <= 1)
    {
        float r = glm::length(nPosition - locPosition);
        float leftOp = 4 / (PI * pow(smoothingLength, 8));
        float rightOp = pow(pow(smoothingLength, 2) - pow(r, 2), 3);
        float res = leftOp * rightOp;
    
        return res;
    }
    else
    {
        return 0;
    }
}


/* Attention */
void FluidParticle::UpdateLocalDensity()
{
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


glm::vec3 FluidParticle::CalWPolyGradient(glm::vec3 r, float h){
    float r_norm = glm::length(r);
    glm::vec3 result;
    if(r_norm<=h) {
        result = -945.0f / (32.0f * PI * (float)glm::pow(h,9)) * (float)glm::pow((float)glm::pow(h,2) - (float)glm::pow(r_norm,2),2) * r;
    }
    return result;
}


float FluidParticle::CalWPolyLaplacian(glm::vec3 r, float h){
    float r_norm = glm::length(r);
    float result = 0;
    if(r_norm<=h) {
        result = 945.0f / (32.0f * PI * (float)glm::pow(h,9)) * ((float)glm::pow(h,2) - (float)glm::pow(r_norm,2)) * (7.0f * (float)glm::pow(r_norm,2) - 3.0f * (float)glm::pow(h,2));
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





void FluidParticle::CalTension(){
    
}

