#ifndef SYS_H
#define SYS_H

#include <vector>
#include <iostream>
#include <armadillo>
#include <stddef.h>
#include <iomanip>
#include <typeinfo>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <map>

#include "filament.h"
#include "clipper.hpp"
#include "points.h"

using namespace ClipperLib;

typedef std::map<std::string, std::string> mapFiles;
typedef std::map <std::string, Node *> mapCurrents;

struct System
{
	std::vector<Filament *> filVect;
	std::vector<Points *> pointVect;
	
	int len;
	int grid_width;
	int fil_len;
	int glyph_view;
	int x_slice;
	int factor;
	int NGrid;

	mapFiles m_files;

    std::string mat_name;
	std::string test_name;
	std::string mat_file;
	std::string fil_file;
	std::string dir;
	std::string gds_name;
	std::string fil_name;
	std::string layer_file;
	std::string gds_file;

	Paths layerpoly;
};

#endif
