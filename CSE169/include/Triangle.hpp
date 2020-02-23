//
//  Triangle.hpp
//  CSE169
//
//  Created by Marino Wang on 2/22/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp
#define GL_SILENCE_DEPRECATION


#include <stdio.h>
#include "Particle.hpp"
#include "core.h"
#include <iostream>
#include <vector>

using namespace std;

class Triangle{
public:
    
    // Member Variables
    Particle* a;
    Particle* b;
    Particle* c;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    glm::vec3 normal;
    
    
    // Member Functions
    Triangle(Particle* a, Particle* b, Particle* c);
    void calNormal();
    //void Update();
    //void Draw(const glm::mat4& viewProjMtx, glm::mat4& modelView, GLuint shader);
    //void DrawSetting();
    
    
    ~Triangle();
};



#endif /* Triangle_hpp */
