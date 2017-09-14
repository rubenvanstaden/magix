#include "sys.h"
#include "clipper.hpp"

#include "geometry.h"
#include "read.h"
#include "grid.h"
#include "vtk_functions.h"
#include "polygons.h"
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
            -v --verbose
            -d --debug
            -h --help
            --version
)";

void vtk_methods(System *sys, Grid *grid, std::string test_name, std::map <std::string, Node *> filpoints, std::string vtk_run)
{
    boost::filesystem::path project_root_path(boost::filesystem::current_path());
    std::string test_dir(project_root_path.string() + "/tests/");
    std::string test_path(test_dir + test_name + "/vtk/");
    std::string efield_run("paraview " + test_path + "efield.vtp");
    std::string hfield_run("paraview " + test_path + "hfield.vtp");
    
    // vtk_grid_polygons(sys, grid);
    // vtk_hfield_struct_grid(sys, grid);
    
    // Calculate the magnetic and current fields.
    vtk_hfield(sys, grid);
    vtk_efield(sys, grid, filpoints);

    if (vtk_run.compare("efield") == 0)
        std::system(efield_run.c_str());
    else if (vtk_run.compare("hfield") == 0)
        std::system(hfield_run.c_str());
}

int main(int argc, char *argv[]) 
{
    System *sys = new System;
    Grid *grid = new Grid;
    Layers *layers = new Layers;

    std::string version = "Magix 1.0.0";

    std::cout << "\n";
    auto args = docopt::docopt(USAGE, {argv+1, argv+argc}, true, version, false);
    for(auto const& arg : args)
        std::cout << arg.first << ": " << arg.second << std::endl;

    sys->glyph_view = 0;
    sys->x_slice = 0.0;

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
        if (args["<port-name>"].isStringList()) {
            std::string test_name = args["<test-name>"].asString();            
            auto const& list = args["<port-name>"].asStringList();

            file_system(sys, test_name, list);
            read_filaments(sys);
       
            std::cout << "\n--- The following .mat files will be used ---" << std::endl;            
            for(auto const& port : list)
                read_mat(sys, port);
            std::cout << "\n";

            mapCurrents filpoints = filament_point_currents(sys);

            bounding_box(sys, grid, layers);
            create_grid_nodes(sys, grid);
    
            clip_layers(sys, layers);
            biot_savart(sys, grid);
            // bicubic(sys);
            // lic(sys, grid);
            // write_bmp(sys, grid);
    
            if (args["--vtk"].isString())
                vtk_methods(sys, grid, test_name, filpoints, args["--vtk"].asString());
        }
    }

    printf("\n%c[1;31mProgram Success\033[0m\n\n", 27);

    return 0;
}














