#ifndef POINTS_H
#define POINTS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <armadillo>
#include <vector>
#include <armadillo>

class Points
{
    public:
        int id;
        std::string layer;

        Node node;        
        Node current;
        
        inline Points();
        inline ~Points();
};

inline Points::Points(){}
inline Points::~Points() {}

#endif