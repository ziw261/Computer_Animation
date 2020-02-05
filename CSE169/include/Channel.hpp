//
//  Channel.hpp
//  CSE169
//
//  Created by Marino Wang on 2/4/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef Channel_hpp
#define Channel_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include "Keyframe.hpp"

class Channel{
    
public:
    
    // Member variables
    float timeStart, timeEnd;
    char extrapIn[256];
    char extrapOut[256];
    int numKeys;
    vector <Keyframe* > keys;
    
    
    // Member functions
    Channel(float s, float e);
    bool Load(Tokenizer &t);
    void TangentCalc();
    void CubicCoeffCalc();
    void Precompute();
    float Evaluate(float currTime);
    int BinaryS(float currTime, int left, int right);
    ~Channel();
    
    
};







#endif /* Channel_hpp */
