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
    int reBounceCounter = 0;
    
    float localDensity;
    float pressure;
    float viscosity;
    const float restDensity = 1.f;
    float supportRadius;
    float smoothingLength;
    const float stiffConst = 1.f;
    const float springConst = 100.0f;
    const float dampFact = 5.0f;
    
    
    
    // Member Function
    FluidParticle(glm::vec3 position, float radius, float supportRadius);
    void Update(float deltaTime);
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    float CalWPoly(glm::vec3 r, float h);
    glm::vec3 CalWPolyGradient(glm::vec3 r, float h);
    float CalWPolyLaplacian(glm::vec3 r, float h);
    glm::vec3 CalWSpikyGradient(glm::vec3 r, float h);
    float CalWViscosityLaplacian(glm::vec3 r, float h);
    void UpdateLocalDensity();
    void UpdatePressure();


    
    void CalTension();
    
    
    void ApplyViscosityForce();
    void ApplyPressureForce();
    void ApplyRepulsion();
    int HandleCollision();
    float Kernel(glm::vec3 nPosition, glm::vec3 locPosition);
    void UpdateForces();
    void HandleImpulse(int collisionType);
    //glm::vec3 GetPressureGradient();
    //glm::vec3 GetViscosityGradient();
    glm::vec3 GetSpikyKernelGradient(float q, glm::vec3 posDiff);
    float GetViscousLagrangian(float q);
    glm::vec3 getKernelGradient(FluidParticle* nBor);
    

    ~FluidParticle();
    
};


#endif /* FluidParticle_hpp */
