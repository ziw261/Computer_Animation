//
//  ParticleSystem.hpp
//  CSE169
//
//  Created by Marino Wang on 2/20/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp
#define GL_SILENCE_DEPRECATION



#include <stdio.h>
#include <iostream>
#include "core.h"
#include "Particle.hpp"
#include "Triangle.hpp"
#include <vector>

using namespace std;


class ParticleSystem{
public:
    
    //Member Variables
    int numParticles;
    float width, divDis;
    vector<Particle*> particles;
    glm::vec3 gravity = glm::vec3(0,-9.8f,0);
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<Triangle*> triangles;
    vector<int> triangleIndex;
    
    
    //Member Functions
    ParticleSystem(float width);
    void Update(float deltaTime);
    void DrawSetting();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void ZeroForce();
    void ApplyGravity();
    void InitParticles();
    void InitTriangles();
};


#endif /* ParticleSystem_hpp */
