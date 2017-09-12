#ifndef LAYERS_H
#define LAYERS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <armadillo>
#include <vector>
#include <armadillo>

struct Poly
{
    int id;
    // tl, bl, br, tr
    Node corners[4];
};

class Layers
{
    public:
        std::vector<Poly *> layVect;
        
        inline Layers();
        inline ~Layers();
};

inline Layers::Layers() {}
inline Layers::~Layers() {}

#endif