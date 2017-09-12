#include "sys.h"
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
#include <math.h>

using namespace ClipperLib;

static const char *const USAGE =
    R"(Usage: 
        magix <test-name> --ports <port-name>...
        magix <test-name> [--vtk=<vtk-run>] --ports <port-name>...        
        magix <test-name> [--vtk=<vtk-run>] --grid=[small | medium | large] --ports <port-name>...

        Options:
            -v --verbose        Verbose mode
            -d --debug          Debugging mode
            -h --help
            --version
)";

void vtk_methods(System *sys, Grid *grid, std::map <std::string, Node *> filpoints, std::string vtk_run)
{
    vtk_grid_polygons(sys, grid);
    vtk_mag_field(sys, grid);
    vtk_poly_points(sys, grid);
    vtk_current_sum(sys, grid, filpoints);

    if (vtk_run.compare("curr") == 0)
        std::system("paraview ../vtk_sum_current.vtp");
    else if (vtk_run.compare("mag") == 0)
        std::system("paraview ../vtk_magnetic_field.vtp");
}

std::map <std::string, Node *> filament_point_currents(System *sys)
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

        std::string nodekey = std::to_string(p->x) + " " + std::to_string(p->y) + " " + std::to_string(p->z);

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
    if (args["--grid"].isString()) {
        std::string gridsize = args["--grid"].asString();

        if (gridsize.compare("small") == 0) {
            sys->factor = 100;
            sys->NGrid = 100;
        }
        else if (gridsize.compare("medium") == 0) {
            sys->factor = 500;
            sys->NGrid = 500;
        }
        else if (gridsize.compare("large") == 0) {
            sys->factor = 1000;
            sys->NGrid = 1000;
        }
    } else {
        sys->factor = 10;
        sys->NGrid = 10;    
    }

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

            mapCurrents filpoints = filament_point_currents(sys);

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














