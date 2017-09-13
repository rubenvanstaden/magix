#include <string.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <float.h>

#include "clipper.hpp"
#include "filament.h"
#include "geometry.h"

#include "sys.h"
#include "layers.h"
#include "grid.h"

using namespace ClipperLib;

void seg_to_array(System *sys, Layers *layers);

void bounding_box(System *sys, Grid *grid, Layers *layers) 
{
    printf("Calculate bounding box of circuit\n");

    seg_to_array(sys, layers);
    
    Node box_min;
    Node box_max;

    box_min.x = DBL_MAX;
    box_min.y = DBL_MAX;
    box_min.z = DBL_MAX;

    box_max.x = -DBL_MAX;
    box_max.y = -DBL_MAX;
    box_max.z = -DBL_MAX;

    for (int i = 0; i < layers->layVect.size(); i++) {
        Poly *poly = layers->layVect[i];

        for (int j = 0; j < 4; j++) {
            // x direcction max
            if (poly->corners[j].x > box_max.x)
                box_max.x = poly->corners[j].x;
            if (poly->corners[j].x < box_min.x)
                box_min.x = poly->corners[j].x;

            // y direcction max
            if (poly->corners[j].y > box_max.y)
                box_max.y = poly->corners[j].y;
            if (poly->corners[j].y < box_min.y)
                box_min.y = poly->corners[j].y;

            // z direcction max
            if (poly->corners[j].z > box_max.z)
                box_max.z = poly->corners[j].z;
            if (poly->corners[j].z < box_min.z)
                box_min.z = poly->corners[j].z;
        }
    }

    // Maybe the x an y directions have to switch?
    grid->width = (box_max.y - box_min.y) * 2.0;
    grid->height = (box_max.z - box_min.z) * 2.0;

    grid->center.x = box_min.y + grid->width/4.0;
    grid->center.y = box_min.z + grid->height/4.0;
    
    printf("\n(x_min, x_max): %E %E\n", box_min.x, box_max.x);
    printf("(y_min, y_max): %E %E\n", box_min.y, box_max.y);
    printf("(z_min, z_max): %E %E\n", box_min.z, box_max.z);
}

void create_grid_nodes(System *sys, Grid *grid) 
{
    grid->cellwidth = grid->width / sys->NGrid;

    grid->nx.reshape(sys->NGrid, sys->NGrid);
    grid->ny.reshape(sys->NGrid, sys->NGrid);
    grid->nz.reshape(sys->NGrid, sys->NGrid);
    
    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {            
            double x_begin = grid->center.x - grid->width/2.0;
            double y_begin = grid->center.y - grid->width/2.0;

            grid->nx(i,j) = 0.0;            
            grid->ny(i,j) = x_begin + i*grid->cellwidth;
            grid->nz(i,j) = y_begin + j*grid->cellwidth;
        }
    }
}

void seg_to_array(System *sys, Layers *layers) 
{
    printf("Start polygon manipulation Phase 1 using Clippers\n");

    Node *x_box = (Node *) calloc(1, sizeof(Node));
    Node *y_box = (Node *) calloc(1, sizeof(Node));

    for (auto fil : sys->filVect) {
        double slice_dist = fabs(fil->S.x - sys->x_slice);
        double fil_length = fabs(fil->length / 2.0);

        if (slice_dist <= fil_length) {
            Poly *poly = new Poly;

            for (int j = 0; j < 4; j++) {
                poly->corners[j].x = fil->corners[j].x;
                poly->corners[j].y = fil->corners[j].y;
                poly->corners[j].z = fil->corners[j].z;
            }

            layers->layVect.push_back(poly);
        }
    }
    
    printf("End polygon manipulation Phase 1\n");
}

Paths create_poly(Poly *layerpoly) {
    Paths poly(1);

    cInt x[4];
    cInt y[4];

    for (int j = 0; j < 4; j++) {
        x[j] = (int) (layerpoly->corners[j].y);
        y[j] = (int) (layerpoly->corners[j].z);
        poly[0] << IntPoint(x[j], y[j]);
    }

    return poly;
}

void clip_layers(System *sys, Layers *layers) {
    std::cout << "\n* Clipping sliced layers" << std::endl;
    
    Paths subj(1);

    int i, j;
    int count = 0;

    for (i = 0; i < layers->layVect.size(); i++) {
        Paths clip(1);
        
        if (count == 0) {
            subj = create_poly(layers->layVect[i]);
            std::cout << subj << std::endl;            
        }
        else {
            Clipper clpr;            
            clip = create_poly(layers->layVect[i]);

            clpr.AddPaths(subj, ptSubject, true);
            clpr.AddPaths(clip, ptClip, true);
            clpr.Execute(ctUnion, subj, pftEvenOdd, pftEvenOdd);
        }

        count++;
    }

    std::cout << "- " << "\033[1;32m" << "success" << "\033[0m" << std::endl;
    
    sys->layerpoly = subj;
}























