//
//  dof.cpp
//  CSE169
//
//  Created by Marino Wang on 1/13/20.
//  Copyright © 2020 Marino Wang. All rights reserved.
//

#include "dof.hpp"

dof::dof(){
    value = 0;
    min = -100000;
    max = 100000;
}

dof::dof(float a, float b){
    value = 0;
    min = a;
    max = b;
}

float dof::GetValue(){
    return this->value;
}

void dof::SetValue(float value){
    this->value = value;
}

void dof::SetMinMax(float min, float max){
    this->min = min;
    this->max = max;
}

void dof::SetMin(float min){
    this->min = min;
}

void dof::SetMax(float max){
    this->max = max;
}

float dof::GetMin(){
    return this->min;
}

float dof::GetMax(){
    return this->max;
}

void dof::ChangeValue(long value){
    this->value += value;
    if(this->value > max){
        this->value = max;
    } else if(this->value < min){
        this->value = min;
    }
    //std::cout << this->min << std::endl;
    //std::cout << this->value << std::endl;
   
    
}

void dof::AdjustValue(){
    if(value > max){
        value = max;
    } else if(value < min) {
        value = min;
    }
}

dof::~dof(){
    
}

