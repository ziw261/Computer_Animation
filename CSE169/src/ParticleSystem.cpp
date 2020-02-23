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
    divDis = 0.1f;
    
    InitParticles();
    InitTriangles();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);

}

void ParticleSystem::Update(float deltaTime){
    ZeroForce();
    ApplyGravity();
    for(int i=0; i<particles.size(); i++){
        particles[i]->Update(deltaTime);
    }
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

void ParticleSystem::InitParticles(){
    
    //float currZ = width * -1.0f;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            Particle* newPart = new Particle(glm::vec3((j-width/2)*0.2, (width/2-i)*0.2, 0));
            
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
            int indexA = (width * i) + j, indexB = (width * (i - 1)) + (j + 1);
            int indexC = (width * (i - 1)) + j, indexD = (width * (i)) + (j + 1);
            Triangle* upperLTriangle = new Triangle(particles[indexA], particles[indexB], particles[indexC]);
            triangles.push_back(upperLTriangle);
            triangleIndex.push_back(indexA);
            triangleIndex.push_back(indexB);
            triangleIndex.push_back(indexC);
            Triangle* upperRTriangle = new Triangle(particles[indexD], particles[indexB], particles[indexC]);
            triangles.push_back(upperRTriangle);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexB);
            triangleIndex.push_back(indexC);
            Triangle* lowerLTriangle = new Triangle(particles[indexA], particles[indexD], particles[indexC]);
            triangles.push_back(lowerLTriangle);
            triangleIndex.push_back(indexA);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexC);
            Triangle* lowerRTriangle = new Triangle(particles[indexA], particles[indexD], particles[indexB]);
            triangles.push_back(lowerRTriangle);
            triangleIndex.push_back(indexA);
            triangleIndex.push_back(indexD);
            triangleIndex.push_back(indexB);

        }
    }
     /*
    Triangle * t1 = new Triangle(particles[0], particles[1], particles[3]);
    Triangle * t2 = new Triangle(particles[0], particles[1], particles[4]);
    Triangle * t3 = new Triangle(particles[0], particles[3], particles[4]);
    Triangle * t4 = new Triangle(particles[1], particles[3], particles[4]);
    Triangle * t5 = new Triangle(particles[1], particles[2], particles[4]);
    Triangle * t6 = new Triangle(particles[1], particles[2], particles[5]);
    Triangle * t7 = new Triangle(particles[1], particles[4], particles[5]);
    Triangle * t8 = new Triangle(particles[2], particles[4], particles[5]);
    Triangle * t9 = new Triangle(particles[3], particles[4], particles[6]);
    Triangle * t10 = new Triangle(particles[3], particles[4], particles[7]);
    Triangle * t11 = new Triangle(particles[3], particles[6], particles[7]);
    Triangle * t12 = new Triangle(particles[4], particles[6], particles[7]);
    Triangle * t13 = new Triangle(particles[4], particles[5], particles[7]);
    Triangle * t14 = new Triangle(particles[4], particles[5], particles[8]);
    Triangle * t15 = new Triangle(particles[4], particles[7], particles[8]);
    Triangle * t16 = new Triangle(particles[5], particles[7], particles[8]);
    triangleIndex.push_back(0);
    triangleIndex.push_back(1);
    triangleIndex.push_back(3);
    triangleIndex.push_back(0);
    triangleIndex.push_back(1);
    triangleIndex.push_back(4);
    triangleIndex.push_back(0);
    triangleIndex.push_back(3);
    triangleIndex.push_back(4);
    triangleIndex.push_back(1);
    triangleIndex.push_back(3);
    triangleIndex.push_back(4);
    triangleIndex.push_back(1);
    triangleIndex.push_back(2);
    triangleIndex.push_back(4);
    triangleIndex.push_back(1);
    triangleIndex.push_back(2);
    triangleIndex.push_back(5);
    triangleIndex.push_back(1);
    triangleIndex.push_back(4);
    triangleIndex.push_back(5);
    triangleIndex.push_back(2);
    triangleIndex.push_back(4);
    triangleIndex.push_back(5);
    triangleIndex.push_back(3);
    triangleIndex.push_back(4);
    triangleIndex.push_back(6);
    triangleIndex.push_back(3);
    triangleIndex.push_back(4);
    triangleIndex.push_back(7);
    triangleIndex.push_back(3);
    triangleIndex.push_back(6);
    triangleIndex.push_back(7);
    triangleIndex.push_back(4);
    triangleIndex.push_back(6);
    triangleIndex.push_back(7);
    triangleIndex.push_back(4);
    triangleIndex.push_back(5);
    triangleIndex.push_back(7);
    triangleIndex.push_back(4);
    triangleIndex.push_back(5);
    triangleIndex.push_back(8);
    triangleIndex.push_back(4);
    triangleIndex.push_back(7);
    triangleIndex.push_back(8);
    triangleIndex.push_back(5);
    triangleIndex.push_back(7);
    triangleIndex.push_back(8);

*/
    
}



void ParticleSystem::DrawSetting(){
    vertices.clear();
    normals.clear();
    for(int i=0; i<particles.size(); i++){
       
        vertices.push_back(particles[i]->position);
        
        normals.push_back(particles[i]->normal);
    }
    //cerr << particles.size() << endl;

    
    
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
    glm::vec3 color = glm::vec3(1.0f, 0, 0);

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





