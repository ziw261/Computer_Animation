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
                Vector3 tempPos = Vector3();
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
                Vector3 tempNorm = Vector3();
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
                glm::mat4 tempMat = glm::mat4(1.0f);
                
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
            return true;
        }
        
        else {
            t.SkipLine();
        }
    }
}
