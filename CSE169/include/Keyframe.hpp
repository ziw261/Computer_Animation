//
//  Keyframe.hpp
//  CSE169
//
//  Created by Marino Wang on 2/4/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Keyframe_hpp
#define Keyframe_hpp

#include <stdio.h>
#include "Tokenizer.h"



using namespace std;

class Keyframe{
    
public:
    
    // Member variables
    float time;
    float value;
    float tangentIn;
    float tangentOut;
    char ruleIn[256];
    char ruleOut[256];
    float a,b,c,d;
    
    
    // Member functions
    Keyframe();
    bool Load (Tokenizer &t);
    ~Keyframe();
    
    
};



#endif /* Keyframe_hpp */
