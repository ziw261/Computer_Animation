//
//  ParticleSystem.cpp
//  CSE169
//
//  Created by Marino Wang on 2/20/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(float width){
    
    this->width = width;
    unitLen = 0.1f;
    airVelocity = glm::vec3(0.0f,0.0f,-15.0f);
    
    InitParticles();
    InitTriangles();
    InitSpringDamper();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);

}

void ParticleSystem::Update(float deltaTime){
    ZeroForce();
    ApplyGravity();
    ApplyWind();
    UpdateSpringDamper();
    UpdateParticles(deltaTime);
    for(int j=0; j<particles.size(); j++){
        particles[j]->normal = glm::vec3(0);
    }
    for(int i=0; i<triangles.size(); i++){
        triangles[i]->CalNormal();
    }
    UpdateVertNorm();
    DrawSetting();
}

void ParticleSystem::ZeroForce(){
    for(int i=0; i<particles.size(); i++){
        particles[i]->ZeroForce();
    }
}

void ParticleSystem::ApplyGravity(){
    for(int i=0; i<particles.size(); i++){
        particles[i]->ApplyForce(gravity);
    }
}


void ParticleSystem::ApplyWind(){
    for(int i=0; i<triangles.size(); i++){
        triangles[i]->ApplyAeroDynaForce(airVelocity);
    }
}



void ParticleSystem::MoveFixedPoint(glm::vec3 distance){
    for(int i=0; i<width; i++){
        particles[i]->position = particles[i]->position+distance;
    }
}


void ParticleSystem::ReleaseFixedPoint(){
    for(int i=0; i<width; i++){
        particles[i]->isFixed = false;
    }
}

void ParticleSystem::InitParticles(){
    
    //float currZ = width * -1.0f;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            Particle* newPart = new Particle(glm::vec3((j-width/2)*unitLen, (width/2-i)*unitLen, 0));
            if (i == 0){
                newPart->isFixed = true;
            }
             
            particles.push_back(newPart);
        }
    }
     
    /*
    particles.clear();
    particles.push_back(new Particle(glm::vec3(-1,1,0)));
    particles.push_back(new Particle(glm::vec3(-1,2,0)));
    particles.push_back(new Particle(glm::vec3(-1,3,0)));
    particles.push_back(new Particle(glm::vec3(0,1,0)));
    particles.push_back(new Particle(glm::vec3(0,2,0)));
    particles.push_back(new Particle(glm::vec3(0,3,0)));
    particles.push_back(new Particle(glm::vec3(1,1,0)));
    particles.push_back(new Particle(glm::vec3(1,2,0)));
    particles.push_back(new Particle(glm::vec3(1,3,0)));
     */

}

void ParticleSystem::InitTriangles(){
    
    for (int i = 1; i < width; i++)
    {
        for (int j = 0; j < width-1; j++)
        {
            int indexA = (width * (i - 1)) + j;
            int indexB = (width * (i - 1)) + (j + 1);
            int indexC = (width * i) + j;
            int indexD = (width * (i)) + (j + 1);
            Triangle* upperLTriangle = new Triangle(particles[indexC], particles[indexB], particles[indexA]);
            triangles.push_back(upperLTriangle);
            triangleIndex.push_back(indexC);
            triangleIndex.push_back(indexB);
            triangleIndex.push_back(indexA);
            Triangle* upperRTriangle = new Triangle(particles[indexD], particles[indexB], particles[indexA]);
            triangles.push_back(upperRTriangle);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexB);
            triangleIndex.push_back(indexA);
            Triangle* lowerLTriangle = new Triangle(particles[indexC], particles[indexD], particles[indexA]);
            triangles.push_back(lowerLTriangle);
            triangleIndex.push_back(indexC);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexA);
            Triangle* lowerRTriangle = new Triangle(particles[indexC], particles[indexD], particles[indexB]);
            triangles.push_back(lowerRTriangle);
            triangleIndex.push_back(indexC);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexB);

        }
    }

    
}


void ParticleSystem::InitSpringDamper(){
    
    float diagLen = glm::sqrt(pow(unitLen,2) + pow(unitLen,2));
    
    for(int i=1; i<width; i++){
        for(int j=0; j<width-1; j++){
            
            int indexA = (width * (i - 1)) + j;
            int indexB = (width * (i - 1)) + (j + 1);
            int indexC = (width * i) + j;
            int indexD = (width * (i)) + (j + 1);
            
            SpringDamper* vertL = new SpringDamper(particles[indexC], particles[indexA], unitLen);
            springDampers.push_back(vertL);
            SpringDamper* horiT = new SpringDamper(particles[indexA], particles[indexB], unitLen);
            springDampers.push_back(horiT);
            SpringDamper* diagL = new SpringDamper(particles[indexA], particles[indexD], diagLen);
            springDampers.push_back(diagL);
            SpringDamper* diagR = new SpringDamper(particles[indexC], particles[indexB], diagLen);
            springDampers.push_back(diagR);
            
            if(i == width-1){
                SpringDamper* horiB = new SpringDamper(particles[indexC], particles[indexD], unitLen);
                springDampers.push_back(horiB);
            }
            if(j+1 == width-1){
                SpringDamper* vertR = new SpringDamper(particles[indexB], particles[indexD], unitLen);
                springDampers.push_back(vertR);
            }
        }
    }
}


void ParticleSystem::UpdateParticles(float deltaTime){
    for(int i=0; i<particles.size(); i++){
        particles[i]->Update(deltaTime);
    }
}


void ParticleSystem::UpdateSpringDamper(){
    for(int i=0; i<springDampers.size(); i++){
        springDampers[i]->ComputeForce();
    }
}



void ParticleSystem::UpdateVertNorm(){
    vertices.clear();
    normals.clear();
    for(int i=0; i<particles.size(); i++){
       
        vertices.push_back(particles[i]->position);
        
        normals.push_back(glm::normalize(particles[i]->normal));
        //cerr << particles.size() << endl;

    }
}



void ParticleSystem::DrawSetting(){
    
    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    
    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndex[0]) * triangleIndex.size(), triangleIndex.data(), GL_STATIC_DRAW);
    
    
    
    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void ParticleSystem::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    // actiavte the shader program
    glm::mat4 model =  glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);

    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    

    // Bind the VAO
    glBindVertexArray(VAO);
    
    
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, triangleIndex.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}



ParticleSystem::~ParticleSystem(){
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

