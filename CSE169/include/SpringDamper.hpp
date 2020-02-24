//
//  SpringDamper.hpp
//  CSE169
//
//  Created by Marino Wang on 2/23/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#ifndef SpringDamper_hpp
#define SpringDamper_hpp

#include <stdio.h>
#include "Particle.hpp"
#include <vector>
#include <iostream>
#include "core.h"

class SpringDamper{
public:
    
    // Member Variables
    float springConstant;
    float dampingFactor;
    float restLength;
    Particle* p1;
    Particle* p2;
    
    
    // Member Functions
    SpringDamper(Particle* p1, Particle* p2, float restLength);
    void ComputeForce();
    ~SpringDamper();
    
};


#endif /* SpringDamper_hpp */
