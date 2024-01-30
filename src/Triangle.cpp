//
//  Triangle.cpp
//  CSE169
//
//  Created by Marino Wang on 2/22/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Triangle.hpp"


Triangle::Triangle(Particle* a, Particle* b, Particle* c){
    this->a = a;
    this->b = b;
    this->c = c;
    airDensity = 2.0f;
    dragCoeff = 2.0f;
    
    CalNormal();
}

void Triangle::CalNormal(){
    normal = glm::normalize(glm::cross(b->position - a->position, c->position - a->position));
    a->normal += normal;
    b->normal += normal;
    c->normal += normal;
}


void Triangle::ApplyAeroDynaForce(glm::vec3 airVelocity){
    glm::vec3 vSurface = (a->velocity + b->velocity + c->velocity)/3.0f;
    glm::vec3 vRelative = vSurface - airVelocity;
    float a0 = 0.5 * glm::length(glm::cross(b->position - a->position, c->position - a->position));
    float area = a0 * (glm::dot(vRelative, normal) / glm::length(vRelative));
    glm::vec3 fAero = -0.5f * airDensity * (float)pow(glm::length(vRelative),2) * dragCoeff * area * normal;
    a->ApplyForce(fAero/3.0f);
    b->ApplyForce(fAero/3.0f);
    c->ApplyForce(fAero/3.0f);
}


Triangle::~Triangle(){
    delete a;
    delete b;
    delete c;
}




