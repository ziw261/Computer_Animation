//
//  AnimationClip.hpp
//  CSE169
//
//  Created by Marino Wang on 2/8/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef AnimationClip_hpp
#define AnimationClip_hpp

#include <stdio.h>
#include <iostream>
#include "Channel.hpp"
#include "Tokenizer.h"
#include "joint.hpp"


class AnimationClip {
public:
    
    // Member Variables
    vector<Channel* > channels;
    vector<joint* > jointGroup;
    int numChannels;
    bool hasTimeStart;
    int currentTime;
    time_t originalTime;
    
    
    // Member Functions
    AnimationClip(vector<joint* > jointGroup);
    bool Load(const char* name);
    void Update(float time);
    ~AnimationClip();
};



#endif /* AnimationClip_hpp */
