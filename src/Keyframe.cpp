//
//  Keyframe.cpp
//  CSE169
//
//  Created by Marino Wang on 2/4/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "Keyframe.hpp"

Keyframe::Keyframe(){
    
}

bool Keyframe::Load(Tokenizer &t){
    
    time = t.GetFloat();
    value = t.GetFloat();
    
    char temp [256];
    t.GetToken(temp);
    
    if(isdigit(temp[0])){
        strcpy(ruleIn, "fixed");
        strcpy(ruleOut, "fixed");
        
        tangentIn = atof(temp);
        tangentOut = t.GetFloat();          //Potential problem might be here?
    } else {
        strcpy(ruleIn, temp);
        t.GetToken(ruleOut);
    }
    
    return true;
}




