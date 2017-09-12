#ifndef FIELDS_H
#define FIELDS_H

#include "clipper.hpp"
#include "layers.h"
#include "sys.h"
#include "grid.h"
#include <armadillo>

using namespace ClipperLib;

void biot_savart(System *sys, Grid *grid);
void lic(System *sys, Grid *grid);
void write_bmp(System *sys, Grid *grid);

# endif