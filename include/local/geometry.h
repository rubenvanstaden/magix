#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "sys.h"

void set_default_options(System *sys);
void file_system(System *sys, std::string test_name);
double get_dec_place(double value, int decPlace);

#endif
