//
//  SpringDamper.cpp
//  CSE169
//
//  Created by Marino Wang on 2/23/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "SpringDamper.hpp"


SpringDamper::SpringDamper(Particle* p1, Particle* p2, float restLength){
    this->p1 = p1;
    this->p2 = p2;
    this->restLength = restLength;
    this->springConstant = 30000.0f;   // Maybe other
    this->dampingFactor = 80.0f;
}



void SpringDamper::ComputeForce(){
    glm::vec3 v1 = p1->velocity;
    glm::vec3 d1 = p1->position;
    glm::vec3 v2 = p2->velocity;
    glm::vec3 d2 = p2->position;
    
    glm::vec3 diff = d2-d1;
    
    glm::vec3 e = glm::normalize(diff);
    
    float l = glm::length(diff);
    
    float fSpring = -1.0f * springConstant * (restLength - l);

    //cerr << fSpring << endl;
    //cerr << l << endl;
    float fDamping = -1.0f * dampingFactor * (glm::dot(v1, e) - glm::dot(v2,e));
    float fSd = fSpring + fDamping;
    //float fSd = fSpring;
    glm::vec3 fOne = fSd * e;
    glm::vec3 fTwo = -1.0f * fOne;
    
    p1->ApplyForce(fOne);
    p2->ApplyForce(fTwo);
}

SpringDamper::~SpringDamper(){
    delete p1;
    delete p2;
}
