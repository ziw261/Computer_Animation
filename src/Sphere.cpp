//
//  Sphere.cpp
//  CSE169
//
//  Created by Marino Wang on 3/7/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Sphere.hpp"

Sphere::Sphere(float radius){
    
    numLon = 30;
    numLat = 30;
    
    this->radius = radius;
    CreateVerts();
    CreateIndics();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
    
    DrawSetting();
}



void Sphere::DrawSetting(){
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
   
   
   
    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::Draw(const glm::mat4 &viewProjMtx, GLuint shader){
    // actiavte the shader program
    //glm::mat4 model =  glm::mat4(1.0f);
    
    glm::mat4 model = toWorld;
   
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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Sphere::CreateVerts(){
    float PI = glm::pi<float>();
    
    for(int i=0; i<=numLat; i++){
        
        float theta = (i * PI) / numLat;
        
        for(int j=0; j<=numLon; j++){
            float phi = (j*2*PI)/numLon;
            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);
            
            vertices.push_back(glm::vec3(x,y,z));
            normals.push_back(glm::normalize(glm::vec3(x,y,z)));
        }
    }
}

void Sphere::CreateIndics(){
    for(int i=0; i<numLat; i++){
        for(int j=0; j<numLon; j++){
            
            int first = (i*(numLon+1))+j;
            int second = first + (numLon + 1);
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first+1);
            
            indices.push_back(second);
            indices.push_back(second+1);
            indices.push_back(first+1);
        }
    }
}


Sphere::~Sphere(){
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
