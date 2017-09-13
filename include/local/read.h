#ifndef READ_H
#define READ_H

#include "sys.h"
#include <string>

void read_mat(System *sys, std::string mat_name);    
void read_filaments(System *sys);
std::map <std::string, Node *> filament_point_currents(System *sys);

#endif