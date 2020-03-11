//
//  Particle.hpp
//  CSE169
//
//  Created by Marino Wang on 2/20/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "core.h"

using namespace std;

class Particle{
public:
    
    // Member Variables
    float mass;
    bool isFixed;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 normal;
    //vector<glm::vec3> normals;
    
    
    // Member Functions
    Particle();
    Particle(glm::vec3 position);
    void Update(float deltaTime);
    void Draw();
    void ApplyForce(glm::vec3 f);
    void ZeroForce();
    void CalNorm();
    
    ~Particle();
    
};



#endif /* Particle_hpp */
