#ifndef VTK_FUNCTIONS_H
#define VTK_FUNCTIONS_H

#include "clipper.hpp"

using namespace ClipperLib;

void vtk_grid_polygons(System *sys, Grid *grid); 
void vtk_mag_field(System *sys, Grid *grid);
void vtk_poly_points(System *sys, Grid *grid);
void vtk_current_sum(System *sys, Grid *grid, std::map <std::string, Node *> filpoints);

# endif