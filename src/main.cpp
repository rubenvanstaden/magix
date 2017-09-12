#include <stdio.h>
#include <stdlib.h>
#include "sys.h"
#include <math.h>
#include "clipper.hpp"

#include "geometry.h"
#include "read.h"
#include "grid.h"
#include "vtk_functions.h"
#include "polygons.h"
#include "layers.h"
#include "fields.h"
#include "points.h"

#include <docopt/docopt.h>
#include <cstdlib>

using namespace ClipperLib;

static const char *const USAGE =
    R"(Usage: 
        magix <test-name> [--vtk=<vtk-run>] --ports <port-name>...

        Options:
            -v --verbose        Verbose mode
            -d --debug          Debugging mode
            -h --help
            --version
)";

void vtk_methods(System *sys, Grid *grid, std::map <std::string, Node *> filpoints, std::string vtk_run)
{
    // vtk_grid_polygons(sys, grid, layerpoly);
    // vtk_layers(sys, grid, layerpoly);
    // vtk_grid(sys, grid, layerpoly);
    // vtk_point_inside();
    vtk_grid_polygons(sys, grid);
    vtk_mag_field(sys, grid);
    // vtk_rect_currents(sys, grid);
    // vtk_corners(sys, grid);
    // vtk_faces(sys, grid);
    vtk_current(sys, grid);
    vtk_poly_points(sys, grid);
    vtk_x_filaments(sys, grid);
    // vtk_clipper(sys, grid);
    vtk_intersection(sys, grid);
    // vtk_filament_intersections(sys, grid);
    // vtk_bool_mapper(sys, grid);
    vtk_current_sum(sys, grid, filpoints);

    else if (vtk_run.compare("curr") == 0)
        std::system("paraview ../magix_current.vtp");
    else if (vtk_run.compare("mag") == 0)
        std::system("paraview ../magix_points.vtp");
}

std::map <std::string, Node *> layer_points(System *sys)
{
    std::map <std::string, Node *> filpoints;

    int count = 0;
    
    for (auto fil : sys->filVect) {
        Node *p = new Node;
        Node *c = new Node;

        p->x = fil->S.x;
        p->y = fil->S.y;
        p->z = fil->S.z;
        
        c->x = fil->stroom.x;
        c->y = fil->stroom.y;
        c->z = fil->stroom.z;

        if (filpoints[nodekey] == 0) {
            filpoints[nodekey] = c;
        }
        else {
            Node *c_prev = filpoints[nodekey];
            c->x += c_prev->x;
            c->y += c_prev->y;
            c->z += c_prev->z;
            filpoints[nodekey] = c;
        }
    }

    return filpoints;
}

int main(int argc, char *argv[]) 
{
    System *sys = new System;
    Grid *grid = new Grid;
    Layers *layers = new Layers;

    auto args = docopt::docopt(USAGE, {argv+1, argv+argc}, true, "Magix 1.0.0", false);
    
    for(auto const& arg : args)
        std::cout << arg.first << ": " << arg.second << std::endl;

    sys->glyph_view = 0;
    sys->x_slice = 0.0;

    // Change these values.
    sys->factor = 50;
    sys->NGrid = 50;

    if (args["<test-name>"].isString()) {
        std::string test_name = args["<test-name>"].asString();

        file_system(sys, test_name);
        read_filaments(sys);

        for (auto fil : sys->filVect) {
            fil->stroom.x = 0.0;
            fil->stroom.y = 0.0;
            fil->stroom.z = 0.0;        
        }    

        if (args["<port-name>"].isStringList()) {
            auto const& list = args["<port-name>"].asStringList();
       
            std::cout << "\n--- The following .mat files will be used ---" << std::endl;            
            for(auto const& port : list)
                read_mat(sys, port);
            std::cout << "\n";

            mapCurrents filpoints = layer_points(sys);

            bounding_box(sys, grid, layers);
            create_grid_nodes(sys, grid);
    
            clip_layers(sys, layers);
            biot_savart(sys, grid);
            // // bicubic(sys);
            
            // lic(sys, grid);
            // write_bmp(sys, grid);
    
            if (args["--vtk"].isString())
                vtk_methods(sys, grid, filpoints, args["--vtk"].asString());
        }
    }

    printf("\n%c[1;31mProgram Success\033[0m\n\n", 27);

    return 0;
}














