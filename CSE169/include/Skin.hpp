//
//  Skin.hpp
//  CSE169
//
//  Created by Marino Wang on 1/26/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Skin_hpp
#define Skin_hpp

#include <stdio.h>
#include "Tokenizer.h"
#include <vector>
#include <iostream>
#include "matrix34.h"
#include "vector3.h"


using namespace std;

class Skin{

public:
    
    // Member Variables
    vector<Vector3> vertices;
    vector<Vector3> normals;
    vector<int> triangles;
    vector<vector<pair<int,float>>> skinweights;
    vector<glm::mat4> bindings;
    
    
    
    // Methods
    Skin();
    ~Skin();
    bool Load(const char* fname);
    void Update();
    void Draw();
    void SkinSmoothAlg();
    
};


#endif /* Skin_hpp */
