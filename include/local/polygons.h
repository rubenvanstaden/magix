#ifndef CROSS_SECTION_H
#define CROSS_SECTION_H

#include "clipper.hpp"
#include "layers.h"
#include "grid.h"

using namespace ClipperLib;

void bounding_box(System *sys, Grid *grid, Layers *layers);
void create_grid_nodes(System *sys, Grid *grid);
void clip_layers(System *sys, Layers *layers);
    
# endif