#ifndef FILAMENT_H
#define FILAMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <armadillo>
#include <vector>
#include <armadillo>

struct Node
{
    int id;
	double x;
	double y;
	double z;
};

class Filament
{
    public:
        arma::field<arma::rowvec> evector;
        arma::field<arma::rowvec> emag;

        int id;
        bool sliced;
        std::string layer;

        double length;
        double width;
        double height;
        double area;

        Node S;
        Node H;
        Node W;
        Node L;

        Node corners[8];
        Node poly[4][4];

        Node begin_node;
        Node end_node;
        Node stroom;
        
        inline Filament();
        inline ~Filament();
};

inline Filament::Filament(){}
inline Filament::~Filament() {}

#endif