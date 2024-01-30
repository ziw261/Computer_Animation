//
//  skeleton.cpp
//  CSE169
//
//  Created by Marino Wang on 1/14/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "skeleton.hpp"

Skeleton::Skeleton(){
    
}

bool Skeleton::Load(const char* fname){
    Tokenizer t =  Tokenizer();
    t. Open(fname);
    t. FindToken("balljoint");
    char tempName[256];
    t.GetToken(tempName);
    joint* jt = new joint();
    jt->SetName(tempName);
    this->SetRoot(jt);
    this->GetRoot()->Load(t);
    t.Close();
    return true;
}

void Skeleton::SetRoot(joint* joint){
    this->root = joint;
}

joint* Skeleton::GetRoot(){
    return this->root;
}

void Skeleton::Update(){

    root->Update(Matrix34::IDENTITY);
    
    // For Fun
    //root->Update(root->GetWorldMatrix());
}

void Skeleton::Draw(const glm::mat4& viewProjMtx, GLuint shader){
    this->GetRoot()->Draw(viewProjMtx, shader);
}

void Skeleton::FillGroup(joint* joint){
    jointGroup.push_back(joint);
    if(joint->HasChildren()){
        for(int i=0; i<joint->GetChildren().size();i++){
            FillGroup(joint->GetChildren()[i]);
        }
    }
    
}


vector<joint*> Skeleton::GetJointGroup(){
    FillGroup(root);
    return jointGroup;
}

long Skeleton::GetGroupSize(){
    return jointGroup.size();
}


Skeleton::~Skeleton(){
    delete root;
}


