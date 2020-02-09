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
    
    if(numKeys == 1){
        return;
    }
    
    for(int i=0; i<numKeys; i++){
        
        // Calculation of Tangent Rule In
        if(strcmp(keys[i]->ruleIn, "flat") == 0){
            keys[i]->tangentIn = 0.0;
        }
        
        else if(strcmp(keys[i]->ruleIn, "linear") == 0){
            
            // First key
            if(i == 0){
                keys[i]->tangentIn = 0;
            } else {
                keys[i]->tangentIn = (keys[i]->value - keys[i-1]->value)/(keys[i]->time - keys[i-1]->time);
            }
        }
        
        else if(strcmp(keys[i]->ruleIn, "smooth") == 0){
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
        if(strcmp(keys[i]->ruleOut, "flat") == 0){
            keys[i]->tangentOut = 0.0;
        }
        
        else if(strcmp(keys[i]->ruleOut, "linear") == 0){
            if(i == numKeys-1){
                keys[i]->tangentOut = 0;
            } else {
                keys[i]->tangentOut = (keys[i+1]->value - keys[i]->value)/(keys[i+1]->time - keys[i]->time);
            }
        }
        
        else if(strcmp(keys[i]->ruleOut, "smooth") == 0){
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

void Channel::CubicCoeffCalc(){
    
    glm::mat4 temp = glm::mat4(2,-3,0,1,-2,3,0,0,1,-2,1,0,1,-1,0,0);
    
    for(int i=0; i<(numKeys-1); i++){
        glm::vec4 temp2 = glm::vec4(keys[i]->value, keys[i+1]->value, (keys[i+1]->time - keys[i]->time) * keys[i]->tangentOut, (keys[i+1]->time - keys[i]->time) * keys[i+1]->tangentIn);
        glm::vec4 result = temp * temp2;
        keys[i]->a = result[0];
        keys[i]->b = result[1];
        keys[i]->c = result[2];
        keys[i]->d = result[3];
    }
}

void Channel::Precompute(){
    TangentCalc();
    CubicCoeffCalc();
}
 
float Channel::FinalCalc(int index, float currTime){
    float u = (currTime - keys[index]->time)/(keys[index+1]->time - keys[index]->time);
    float x = keys[index]->d + u*(keys[index]->c + u*(keys[index]->b + u*keys[index]->a));
    return x;
}


float Channel::Evaluate(float time){
    if(time < keys[0]->time){
        //Extrapolate(time, 0);
        if(strcmp(extrapIn, "constant") == 0){
            return keys[0]->value;
        }
        else if(strcmp(extrapIn, "linear") == 0){
            return keys[0]->value - (keys[0]->time - time)*keys[0]->tangentOut;
        }
        else if(strcmp(extrapIn, "cycle") == 0){
            return Evaluate(keys[numKeys-1]->time - keys[0]->time + time);
        }
        else if(strcmp(extrapIn, "cycle_offset") == 0){
            return Evaluate(keys[numKeys-1]->time - keys[0]->time + time) - (keys[numKeys-1]->value - keys[0]->value);
        }
        else if(strcmp(extrapIn, "bounce") == 0){
            return Evaluate(keys[0]->time + keys[0]->time - time);
        }
    }
    else if(time > keys[numKeys-1]->time){
        //Extrapolate(time, 1);
        if(strcmp(extrapOut, "constant") == 0){
            return keys[numKeys-1]->value;
        }
        else if(strcmp(extrapOut, "linear") == 0){
            return keys[numKeys-1]->value + (time - keys[numKeys-1]->time)*keys[numKeys-1]->tangentIn;
        }
        else if(strcmp(extrapOut, "cycle") == 0){
            return Evaluate(keys[0]->time + time - keys[numKeys-1]->time);
        }
        else if(strcmp(extrapOut,"cycle_offset") == 0){
            return Evaluate(keys[0]->time + time - keys[numKeys-1]->time) + (keys[numKeys-1]->value - keys[0]->value);
        }
        else if(strcmp(extrapOut, "bounce") == 0){
            return Evaluate(keys[numKeys-1]->time - time + keys[numKeys-1]->time);
        }
    }
    else if(numKeys > 1){
        int i;
        for(i=0; i<numKeys-1; i++){
            if(time == keys[i]->time){
                return keys[i]->value;
            }
            else if(time == keys[i+1]->time){
                return keys[i+1]->value;
            }
            else if(time > keys[i]->time && time < keys[i+1]->time){
                break;
            }
            else {
                continue;
            }
        }
        return FinalCalc(i, time);
    }
    else if(numKeys == 1){
        return keys[0]->value;
    }
    
    return -100;

}





