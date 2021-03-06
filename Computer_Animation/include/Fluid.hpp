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
#include "Cube.h"

using namespace std;

class Fluid{
public:
    // Member Variables
    vector<FluidParticle*> fluidParticles;
    float radius;
    float supportRadius;
    glm::vec3 gravity = glm::vec3(0,-9.8f,0);
    const float springConst = 100.f;
    const float dampFact = 0.f;
    bool shouldBeBound = true;    //false
    int gridSize = 8;
    float maxXZ = 0.f;
    float minXZ = 0.f;
    float minY = 0.f;
    float sideHeight = 0.f;
    
    int numParticles;
    Cube* boundCube;
    
    // Member Functions
    Fluid();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void CreateSpheres();
    void Update(float deltaTime);
    void ZeroForce();
    void UpdateParticles(float deltaTime);
    void ApplyGravity();
    void ApplyBound();
    
    
    void UpdateNeighbors();
    void ClearNeighbors();
    void UpdateLocalDensities();
    void UpdateParticleForces();
    ~Fluid();
};


#endif /* Fluid_hpp */
