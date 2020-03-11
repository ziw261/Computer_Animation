//
//  joint.cpp
//  CSE169
//
//  Created by Marino Wang on 1/13/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "joint.hpp"
joint::joint(){
    boxmin.x = -0.1;
    boxmin.y = -0.1;
    boxmin.z = -0.1;
    boxmax.x = 0.1;
    boxmax.y = 0.1;
    boxmax.z = 0.1;
    
    local = Matrix34::IDENTITY;
    world = Matrix34::IDENTITY;
    
    rotxlimit = dof();
    rotylimit = dof();
    rotzlimit = dof();
    
}

void joint::SetName(string nm){
    this->name = nm;
}

void joint::AddChild(joint *jnt){
    this->children.push_back(jnt);
}

Matrix34 joint::GetWorldMatrix(){
    return world;
}

bool joint::Load(Tokenizer &t){
    //t.FindToken("balljoint");
    t.FindToken("{");
    while(1){
        char temp[256];
        t.GetToken(temp);
        if(strcmp(temp,"offset")==0){
            offset.x = t.GetFloat();
            offset.y = t.GetFloat();
            offset.z = t.GetFloat();
        }
        
        else if(strcmp(temp, "pose")==0){

            rotxlimit.SetValue(t.GetFloat());
            rotylimit.SetValue(t.GetFloat());
            rotzlimit.SetValue(t.GetFloat());
           
        }
        
        else if(strcmp(temp, "boxmin")==0){
            boxmin.x = t.GetFloat();
            boxmin.y = t.GetFloat();
            boxmin.z = t.GetFloat();
        }
        
        else if(strcmp(temp, "boxmax")==0){
            boxmax.x = t.GetFloat();
            boxmax.y = t.GetFloat();
            boxmax.z = t.GetFloat();
        }
        
        else if(strcmp(temp, "rotxlimit")==0){

            rotxlimit = dof(t.GetFloat(), t.GetFloat());
            
        }
        
        else if(strcmp(temp, "rotylimit")==0){
            
               rotylimit = dof(t.GetFloat(), t.GetFloat());
            
        }
        
        else if(strcmp(temp, "rotzlimit")==0){

               rotzlimit = dof(t.GetFloat(), t.GetFloat());
            
        }
        
        else if(strcmp(temp,"balljoint")==0){
            joint *jnt = new joint();
            char jointName[256];
            t.GetToken(jointName);
            jnt->name = jointName;
            jnt -> Load(t);
            AddChild(jnt);
         
        }
        
        else if(strcmp(temp, "}")==0){
            return true;
        }
        
        else {
            t.SkipLine();
        }
    }
}

void joint::Update(Matrix34 matrix){
    
    rotxlimit.AdjustValue();
    rotylimit.AdjustValue();
    rotzlimit.AdjustValue();

    local.FromEulers(rotxlimit.GetValue(), rotylimit.GetValue(), rotzlimit.GetValue(), Matrix34::EULER_XYZ);

    local.d.Set(offset.x, offset.y, offset.z);
    world.Dot(matrix, local);
    for(int i=0; i<this->children.size(); i++){
        children[i]->Update(world);
    }
    
}

void joint::Draw(const glm::mat4& viewProjMtx, GLuint shader){

    Matrix34 temp = world;
    glm::mat4 worldM = glm::mat4(temp.a.x, temp.a.y, temp.a.z,0, temp.b.x, temp.b.y, temp.b.z, 0, temp.c.x, temp.c.y, temp.c.z,0,temp.d.x, temp.d.y, temp.d.z, 1);
    
    Cube *c1 = new Cube(glm::vec3(boxmin.x, boxmin.y, boxmin.z), glm::vec3(boxmax.x, boxmax.y, boxmax.z));
    c1->draw(viewProjMtx*worldM, shader);
    for(int i=0; i<this->children.size(); i++){
        children[i]->Draw(viewProjMtx, shader);
    }
}

vector<joint*>joint::GetChildren(){
    return this->children;
}


bool joint::HasChildren(){
    if(children.size() == 0){
        return false;
    } else {
        return true;
    }
}

void joint::ChangeDofX(float value){
    this->rotxlimit.ChangeValue(value);
}

void joint::ChangeDofY(float value){
    this->rotylimit.ChangeValue(value);
}

void joint::ChangeDofZ(float value){
    this->rotzlimit.ChangeValue(value);
}


string joint::GetName(){
    return name;
}

joint::~joint(){
    delete mother;
    delete local;
    delete world;
}





