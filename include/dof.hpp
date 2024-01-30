//
//  dof.hpp
//  CSE169
//
//  Created by Marino Wang on 1/13/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef CSE169_dof_hpp
#define CSE169_dof_hpp

#include <stdio.h>
#include <iostream>

class dof {

private:


public:
    float value = 0;
    float min = -100000;
    float max = 100000;
    dof();
    dof(float a, float b);
    void SetValue(float value);
    float GetValue();
    void SetMinMax(float min, float max);
    void SetMin(float min);
    void SetMax(float max);
    float GetMin();
    float GetMax();
    void ChangeValue(float value);
    void AdjustValue();
    ~dof();
};

#endif /* dof_hpp */
