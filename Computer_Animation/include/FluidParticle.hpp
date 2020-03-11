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
    const float restDensity = 5.f;  // 7.0f     //1.0f
    float supportRadius;
    float smoothingLength;
    const float stiffConst = 5.f;  //5.0f       //1.0f
    const float springConst = 10.0f;
    const float dampFact = 0.0f;
    bool isFreed = false;        // true
    
    
    
    // Member Function
    FluidParticle(glm::vec3 position, float radius, float supportRadius);
    void Update(float deltaTime);
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    float CalWPoly(glm::vec3 r, float h);
    glm::vec3 CalWSpikyGradient(glm::vec3 r, float h);
    float CalWViscosityLaplacian(glm::vec3 r, float h);
    void UpdateLocalDensity();
    void UpdatePressure();

    
    void ApplyViscosityForce();
    void ApplyPressureForce();
    void ApplyRepulsion();
    void UpdateForces();    

    ~FluidParticle();
    
};


#endif /* FluidParticle_hpp */
