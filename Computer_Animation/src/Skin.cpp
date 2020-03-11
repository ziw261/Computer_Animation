//
//  Skin.cpp
//  CSE169
//
//  Created by Marino Wang on 1/26/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Skin.hpp"

Skin::Skin(){
    
}

Skin::Skin(vector<joint*> joints){
    jointGroup = joints;
}


bool Skin::Load(const char* fname){
    
    Tokenizer t = Tokenizer();
    t.Open(fname);
    
    while(true){
        
        char temp[256];
        t.GetToken(temp);
        
        if(strcmp(temp, "positions") ==0) {
            int numPos = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numPos; i++){
                glm::vec3 tempPos = glm::vec3(0,0,0);
                tempPos.x = t.GetFloat();
                tempPos.y = t.GetFloat();
                tempPos.z = t.GetFloat();
                vertices.push_back(tempPos);
            }
            t.FindToken("}");
        }
        
        else if(strcmp(temp, "normals") ==0) {
            int numNorms = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numNorms; i++){
                glm::vec3 tempNorm = glm::vec3(0,0,0);
                tempNorm.x = t.GetFloat();
                tempNorm.y = t.GetFloat();
                tempNorm.z = t.GetFloat();
                normals.push_back(tempNorm);
            }
            t.FindToken("}");
        }
        
        else if(strcmp(temp, "skinweights") ==0) {
            int numWeights = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numWeights; i++){
                int numPairs = t.GetInt();
                vector<pair<int, float>> tempVec;
                for(int j=0; j<numPairs; j++){
                    int tempIndex = t.GetInt();
                    float tempWeight = t.GetFloat();
                    tempVec.push_back(make_pair(tempIndex, tempWeight));
                }
                skinweights.push_back(tempVec);
            }
            t.FindToken("}");
        }
        
        else if(strcmp(temp, "triangles") ==0) {
            int numTri = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numTri; i++){
                for(int j=0; j<3; j++){
                    int tempIndice = t.GetInt();
                    triangles.push_back(tempIndice);
                }
            }
            t.FindToken("}");
        }
        
        else if(strcmp(temp, "bindings") ==0) {
            int numBind = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numBind; i++){
                glm::mat4 tempMat = glm::mat4(0.0f);
                
                t.FindToken("{");
                for(int j=0; j<4; j++){
                    for(int k=0; k<3; k++){
                        float tempVal = t.GetFloat();
                        tempMat[j][k] = tempVal;
                    }
                }
                t.FindToken("}");
                tempMat[3][3] = 1.0f;
                bindings.push_back(tempMat);
            }
            t.FindToken("}");
            
            t.Close();
            return true;
        }
        
        else {
            t.SkipLine();
        }
    }
    
}

void Skin::Start(const char* fname){
    
    Load(fname);
    
    // Initialize new vectors (Maybe not a good place)
    vertices_ac = vector<glm::vec3>(vertices.size());
    normals_ac = vector<glm::vec3>(normals.size());
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
    

}

void Skin::SkinSmoothAlg(){
    
    //v' = Sum(wWB-1v) (x,y,z,1)
    //n' = Sum(wWB-1n) (x,y,z,0)
    
    for(int i=0; i<vertices.size(); i++){
        glm::vec3 vertex_ac = glm::vec3(0,0,0);
        glm::vec3 normal_ac = glm::vec3(0,0,0);
        
        long count = skinweights[i].size();
        for(int j=0; j<count; j++){
            
            // Get the joint index
            float jointIndex = skinweights[i][j].first;
            
            // Get w
            float weight = skinweights[i][j].second;
            
            // Calculation of W
            Matrix34 temp = jointGroup[jointIndex]->GetWorldMatrix();
            glm::mat4 worldM = glm::mat4(temp.a.x, temp.a.y, temp.a.z,0, temp.b.x, temp.b.y, temp.b.z, 0, temp.c.x, temp.c.y, temp.c.z,0,temp.d.x, temp.d.y, temp.d.z, 1);
            
            // Calculation of B-1
            glm::mat4 inverseBind = glm::inverse(bindings[jointIndex]);
            
            // Calculation of v , n
            glm::vec4 tempV = glm::vec4(vertices[i], 1.0f);
            glm::vec4 tempN = glm::vec4(normals[i], 0.0f);
            
            // wWB-1v , wwB-1n
            glm::vec3 tempAddv = weight * worldM * inverseBind * tempV;
            glm::vec3 tempAddn = weight * worldM * inverseBind * tempN;
            
            // sum
            vertex_ac = vertex_ac + tempAddv;
            normal_ac = normal_ac + tempAddn;
        }
        
        vertices_ac[i] = vertex_ac;
        normals_ac[i] = glm::normalize(normal_ac);
    }
}

void Skin::DrawSetting(){

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ac[0]) * vertices.size(), vertices_ac.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals_ac[0])* normals.size(), normals_ac.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    
    
    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles[0]) * triangles.size(), triangles.data(), GL_STATIC_DRAW);
     

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Skin::Update(){
    SkinSmoothAlg();
    DrawSetting();
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader){
    // actiavte the shader program
    glm::mat4 model =  glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);

    glEnable(GL_BLEND);

    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

Skin::~Skin(){
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
