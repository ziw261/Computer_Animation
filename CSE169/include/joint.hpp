//
//  joint.hpp
//  CSE169
//
//  Created by Marino Wang on 1/13/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef CSE169_joint_hpp
#define CSE169_joint_hpp

#include "dof.hpp"
#include "vector3.h"
#include "matrix34.h"
#include "core.h"
#include "Tokenizer.h"
#include "Cube.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;


class joint {
private:
    Matrix34 local;
    Matrix34 world;
    vector<dof*> dofList;
    vector<joint*> children;
    joint *mother;
    dof rotxlimit;
    dof rotylimit;
    dof rotzlimit;
    Vector3 offset;
    Vector3 boxmax;
    Vector3 boxmin;
    string name;
    
    
public:
    joint();
    void Update(Matrix34 matrix);
    void AddChild(joint* jnt);
    bool Load(Tokenizer &t);
    void Draw(const glm::mat4& viewProjMtx, GLuint shader);
    void SetName(string nm);
    string GetName();
    vector<joint*> GetChildren();
    //Matrix34* AdjustPose();
    Matrix34 GetWorldMatrix();
    bool HasChildren();
    void ChangeDofX(long value);
    void ChangeDofY(long value);
    void ChangeDofZ(long value);
    ~joint();
    
};


#endif /* joint_hpp */
