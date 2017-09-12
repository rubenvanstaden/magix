#ifndef VTK_FUNCTIONS_H
#define VTK_FUNCTIONS_H

#include "clipper.hpp"

using namespace ClipperLib;

void vtk_grid_polygons(System *sys, Grid *grid); 
void vtk_grid(System *sys, Grid *grid);    
void vtk_mag_field(System *sys, Grid *grid);
void structured_grid(System *sys, Grid *grid);
void vtk_current(System *sys, Grid *grid);
void vtk_poly_points(System *sys, Grid *grid);
void vtk_x_filaments(System *sys, Grid *grid);
void vtk_faces(System *sys, Grid *grid);
void vtk_corners(System *sys, Grid *grid);
void vtk_rect_currents(System *sys, Grid *grid);
void vtk_clipper(System *sys, Grid *grid);
void vtk_intersection(System *sys, Grid *grid);
void vtk_filament_intersections(System *sys, Grid *grid);
void vtk_bool_mapper(System *sys, Grid *grid);
void vtk_current_sum(System *sys, Grid *grid, std::map <std::string, Node *> filpoints);

# endif