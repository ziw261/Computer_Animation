//
//  AnimationClip.cpp
//  CSE169
//
//  Created by Marino Wang on 2/8/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "AnimationClip.hpp"

AnimationClip::AnimationClip(vector<joint* > jointGroup){
    
    this->jointGroup = jointGroup;
    hasTimeStart = false;
}

bool AnimationClip::Load(const char* name){
    Tokenizer t = Tokenizer();
    t.Open(name);
    
    t.FindToken("{");
    float timeStart = 0;
    float timeEnd = 0;
    
    while(true){
        char temp[256];
        t.GetToken(temp);
        
        if(strcmp(temp, "range")==0){
            timeStart = t.GetFloat();
            timeEnd = t.GetFloat();
        }
        else if(strcmp(temp, "numchannels") ==0){
            numChannels = t.GetInt();
        }
        else if(strcmp(temp, "channel") == 0){
            Channel* newChannel = new Channel(timeStart, timeEnd);
            newChannel -> Load(t);
            newChannel -> Precompute();
            channels.push_back(newChannel);
        }
        else if(strcmp(temp, "}") == 0){
            t.Close();
            return true;
        }
        else {
            t.SkipLine();
        }
    }
}


void AnimationClip::Update(float time){
    float xOffset = channels[0]->Evaluate(time);
    float yOffset = channels[1]->Evaluate(time);
    float zOffset = channels[2]->Evaluate(time);
    
    jointGroup[0]->offset.Set(xOffset, yOffset, zOffset);
    
    for(int i=3; i<numChannels-3; i+=3){
        float newDofX = channels[i]->Evaluate(time);
        float newDofY = channels[i+1]->Evaluate(time);
        float newDofZ = channels[i+2]->Evaluate(time);
        
        jointGroup[(i-3)/3]->rotxlimit.value = newDofX;
        jointGroup[(i-3)/3]->rotylimit.value = newDofY;
        jointGroup[(i-3)/3]->rotzlimit.value = newDofZ;
        jointGroup[(i-3)/3]->rotxlimit.AdjustValue();
        jointGroup[(i-3)/3]->rotylimit.AdjustValue();
        jointGroup[(i-3)/3]->rotzlimit.AdjustValue();
        //jointGroup[(i-3)/3]->ChangeDofY(newDofY);
        //jointGroup[(i-3)/3]->ChangeDofZ(newDofZ);
    }
}

