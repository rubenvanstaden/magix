#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include "filament.h"
#include <armadillo>

struct GridNode
{
    Node node;
    Node image;
    std::string layer;
};

struct Hfield
{
    Node mag;
    Node vec;
};

class Grid
{
    public:
        Node center;

        double width;
        double height;
        double cellwidth;

        arma::mat Hx;
        arma::mat Hy;
        arma::mat Hz;

        arma::mat nx;
        arma::mat ny;
        arma::mat nz;

        arma::mat ix;
        arma::mat iy;
        arma::mat iz;
        
        inline Grid();
        inline ~Grid();
};

inline Grid::Grid(){}
inline Grid::~Grid() {}

#endif
