//
//  skeleton.hpp
//  CSE169
//
//  Created by Marino Wang on 1/14/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef skeleton_hpp
#define skeleton_hpp

#include <stdio.h>
#include "joint.hpp"
#include "Tokenizer.h"
#include <iostream>

class Skeleton{
private:
    joint* root;
    vector<joint*> jointGroup;

public:
    Skeleton();
    long GetGroupSize();
    bool Load(const char* fname);
    void Update();
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void SetRoot(joint* joint);
    void FillGroup(joint* joint);
    vector<joint*> GetJointGroup();
    joint* GetRoot();
    ~Skeleton();
};

#endif /* skeleton_hpp */
