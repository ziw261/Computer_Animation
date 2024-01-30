//
//  Skin.hpp
//  CSE169
//
//  Created by Marino Wang on 1/26/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Skin_hpp
#define Skin_hpp
#define GL_SILENCE_DEPRECATION


#include <stdio.h>
#include "Tokenizer.h"
#include <vector>
#include <iostream>
#include "matrix34.h"
#include "vector3.h"
#include "skeleton.hpp"
#include "core.h"


using namespace std;

class Skin{

public:
    
    // Member Variables
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    vector<glm::vec3> vertices;
    vector<glm::vec3> vertices_ac;
    vector<glm::vec3> normals;
    vector<glm::vec3> normals_ac;
    vector<int> triangles;
    vector<vector<pair<int,float>>> skinweights;
    vector<glm::mat4> bindings;
    vector<joint*> jointGroup;

    
    
    // Methods
    Skin();
    Skin(vector<joint*> joints);
    ~Skin();
    bool Load(const char* fname);
    void Update();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void SkinSmoothAlg();
    void Start(const char* fname);
    void DrawSetting();
    
};


#endif /* Skin_hpp */
