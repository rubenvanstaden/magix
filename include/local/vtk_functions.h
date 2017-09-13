#ifndef VTK_FUNCTIONS_H
#define VTK_FUNCTIONS_H

#include "clipper.hpp"

using namespace ClipperLib;

void vtk_grid_polygons(System *sys, Grid *grid); 
void vtk_hfield_struct_grid(System *sys, Grid *grid);
void vtk_hfield(System *sys, Grid *grid);
void vtk_efield(System *sys, Grid *grid, std::map <std::string, Node *> filpoints);

# endif