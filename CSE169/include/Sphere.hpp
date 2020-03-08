//
//  Sphere.hpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp
#define GL_SILENCE_DEPRECATION



#include <stdio.h>
#include <iostream>
#include "core.h"

using namespace std;

class Sphere{
public:
    
    // Member Variables
    int numLon;        // y
    int numLat;        // x
    float radius;
    glm::mat4 toWorld;
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    vector <glm::vec3> vertices;
    vector <glm::vec3> normals;
    vector <int> indices;
    
    // Member Functions
    Sphere(float radius);
    void CreateVerts();
    void CreateIndics();
    void DrawSetting();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    ~Sphere();
    
    
};



#endif /* Sphere_hpp */
