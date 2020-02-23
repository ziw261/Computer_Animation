//
//  Particle.cpp
//  CSE169
//
//  Created by Marino Wang on 2/20/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(glm::vec3 position){
    this->mass = 1.0f;
    this->velocity = glm::vec3(0,0,0);
    this->force = glm::vec3(0,0,0);
    this->normal = glm::vec3(0);
    isFixed = false;
    this->position = position;
}


void Particle::ApplyForce(glm::vec3 f){
    this->force += f;
}

void Particle::ZeroForce(){
    this->force = glm::vec3(0,0,0);
}



void Particle::Update(float deltaTime){
    
    if(!isFixed){
        glm::vec3 acceleration = (1.0f/mass) * force;
        velocity += acceleration*deltaTime;
        position += velocity * deltaTime;
        //cerr << position.x << " " << position.y << " " << position.z << " " << endl;
        //cerr << acceleration.x << " " << acceleration.y << " " << acceleration.z << " " << endl;
        //cerr << force.x << " " << force.y << " " << force.z << " " << endl;

    }
}
