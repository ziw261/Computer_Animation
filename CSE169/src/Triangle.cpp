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
    
    calNormal();
}

void Triangle::calNormal(){
    normal = glm::normalize(glm::cross(b->position - a->position, c->position - a->position));
    a->normal += normal;
    b->normal += normal;
    c->normal += normal;
}




