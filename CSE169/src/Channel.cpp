//
//  Channel.cpp
//  CSE169
//
//  Created by Marino Wang on 2/4/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Channel.hpp"

Channel::Channel(float s, float e){
    timeStart = s;
    timeEnd = e;
}


bool Channel::Load(Tokenizer &t){
    
    t.FindToken("{");
    
    while(1){
        
        char temp[256];
        t.GetToken(temp);
        
        if(strcmp(temp,"extrapolate") == 0){
            t.GetToken(extrapIn);
            t.GetToken(extrapOut);
        }
        
        else if(strcmp(temp, "keys") == 0){
            
            numKeys = t.GetInt();
            t.FindToken("{");
            for(int i=0; i<numKeys; i++){
                Keyframe* tempKey = new Keyframe();
                tempKey->Load(t);
                keys.push_back(tempKey);
            }
            t.FindToken("}");
            
        }
        
        else if(strcmp(temp, "}")==0){
            return true;
        }
        
        else {
            t.SkipLine();
        }
    }
}


void Channel::TangentCalc(){
    
    for(int i=0; i<numKeys; i++){
        
        // Calculation of Tangent Rule In
        if(strcmp(keys[i]->ruleIn, "flat")){
            keys[i]->tangentIn = 0.0;
        }
        
        else if(strcmp(keys[i]->ruleIn, "linear")){
            
            // First key
            if(i == 0){
                keys[i]->tangentIn = 0;
            } else {
                keys[i]->tangentIn = (keys[i]->value - keys[i-1]->value)/(keys[i]->time - keys[i-1]->time);
            }
        }
        
        else if(strcmp(keys[i]->ruleIn, "smooth")){
            if(i == 0){
                keys[i]->tangentIn = 0;
            } else if( i == numKeys-1){
                keys[i]->tangentIn = (keys[i]->value - keys[i-1]->value)/(keys[i]->time - keys[i-1]->time);
            } else {
                keys[i]->tangentIn = (keys[i+1]->value - keys[i-1]->value)/(keys[i+1]->time - keys[i-1]->time);
            }
        }
        
        else {
            cerr << "Bug, no ruleIn match!" << endl;
        }
        
        
        // Calculation of Tangent Rule Out
        if(strcmp(keys[i]->ruleOut, "flat")){
            keys[i]->tangentOut = 0.0;
        }
        
        else if(strcmp(keys[i]->ruleOut, "linear")){
            if(i == numKeys-1){
                keys[i]->tangentOut = 0;
            } else {
                keys[i]->tangentOut = (keys[i+1]->value - keys[i]->value)/(keys[i+1]->time - keys[i]->time);
            }
        }
        
        else if(strcmp(keys[i]->ruleOut, "smooth")){
            if(i == 0){
                keys[i]->tangentOut = (keys[i+1]->value - keys[i]->value)/(keys[i+1]->time - keys[i]->time);
            } else if( i == numKeys-1){
                keys[i]->tangentOut = 0;
            } else {
                keys[i]->tangentOut = (keys[i+1]->value - keys[i-1]->value)/(keys[i+1]->time - keys[i-1]->time);
            }
        }
        
        else {
            cerr << "Bug, no ruleOut match!" << endl;
        }
    }
    
}

