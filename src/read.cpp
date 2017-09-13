#include <string>
#include <iostream>
#include <fstream>

#include "read.h"

#include <boost/algorithm/string.hpp>

void split_line(System *sys, std::vector<std::string> linevect, int num);
void number_of_filaments(System *sys);    

void number_of_filaments(System *sys) 
{
    std::string fil_file = sys->m_files["dir"] + sys->m_files[".fil"];
    std::ifstream myfile(fil_file);
    std::string line;

    int line_num = 0;
    
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            std::stringstream ss(line);
            std::string word;
            std::string linestr(line);
                        
            if (line_num == 3)
                sys->fil_len = atoi(linestr.c_str());

            line_num++;
        }
        
        myfile.close();
    } else
        throw std::invalid_argument("Unable to open MSH file.");
}

void create_fil_corners(System *sys) 
{
    std::cout << "\n--- Creating filament corners ---" << std::endl;
    
    // For now we assume the filaments are lying in one plane.
    // i.o.w. we have to implement algorithms for filaments 
    // that are lying at an angle.
    for (auto fil : sys->filVect) {
        double lx = fil->length*fil->L.x;
        double ly = fil->length*fil->L.y;        

        double wx = fil->width*fil->W.x;
        double wy = fil->width*fil->W.y;

        double h = fil->height*fil->H.z;
        
        if (fil->L.x == 1.0) {
            // Bot-Left
            fil->corners[0].x = fil->S.x;
            fil->corners[0].y = fil->S.y - wy;
            fil->corners[0].z = fil->S.z + h;
            // Bot-Right
            fil->corners[1].x = fil->S.x;
            fil->corners[1].y = fil->S.y - wy;
            fil->corners[1].z = fil->S.z - h;
            // Top-Right
            fil->corners[2].x = fil->S.x;
            fil->corners[2].y = fil->S.y + wy;
            fil->corners[2].z = fil->S.z - h;
            // Top-Left
            fil->corners[3].x = fil->S.x;
            fil->corners[3].y = fil->S.y + wy;
            fil->corners[3].z = fil->S.z + h;

            // Bot-Left
            fil->corners[4].x = fil->S.x - lx;
            fil->corners[4].y = fil->S.y - wy;
            fil->corners[4].z = fil->S.z + h;
            // Bot-Right
            fil->corners[5].x = fil->S.x - lx;
            fil->corners[5].y = fil->S.y - wy;
            fil->corners[5].z = fil->S.z - h;
            // Top-Right
            fil->corners[6].x = fil->S.x - lx;
            fil->corners[6].y = fil->S.y + wy;
            fil->corners[6].z = fil->S.z - h;
            // Top-Left
            fil->corners[7].x = fil->S.x - lx;
            fil->corners[7].y = fil->S.y + wy;
            fil->corners[7].z = fil->S.z + h;
        }

        if (fil->L.y == 1.0) {
            // Bot-Left
            fil->corners[0].x = fil->S.x - wx;
            fil->corners[0].y = fil->S.y;
            fil->corners[0].z = fil->S.z + h;
            // Bot-Right
            fil->corners[1].x = fil->S.x - wx;
            fil->corners[1].y = fil->S.y;
            fil->corners[1].z = fil->S.z - h;
            // Top-Right
            fil->corners[2].x = fil->S.x + wx;
            fil->corners[2].y = fil->S.y;
            fil->corners[2].z = fil->S.z - h;
            // Top-Left
            fil->corners[3].x = fil->S.x + wx;
            fil->corners[3].y = fil->S.y;
            fil->corners[3].z = fil->S.z + h;

            // Bot-Left
            fil->corners[4].x = fil->S.x - wx;
            fil->corners[4].y = fil->S.y - ly;
            fil->corners[4].z = fil->S.z + h;
            // Bot-Right
            fil->corners[5].x = fil->S.x - wx;
            fil->corners[5].y = fil->S.y - ly;
            fil->corners[5].z = fil->S.z - h;
            // Top-Right
            fil->corners[6].x = fil->S.x - wx;
            fil->corners[6].y = fil->S.y + ly;
            fil->corners[6].z = fil->S.z - h;
            // Top-Left
            fil->corners[7].x = fil->S.x - wx;
            fil->corners[7].y = fil->S.y + ly;
            fil->corners[7].z = fil->S.z + h;
        }
    }

    std::cout << "--- Success filament corners ---" << std::endl;
}

void read_filaments(System *sys) 
{
    std::string fil_file = sys->m_files["dir"] + sys->m_files[".fil"];
    std::cout << "\n--- Reading filament file: " << fil_file << std::endl;
    std::ifstream myfile(fil_file);
    std::string line;

    number_of_filaments(sys);    

    int line_num = 0;
    
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            std::stringstream ss(line);
            std::string word;
            std::string linestr(line);

            if (line_num > 3) {
                std::vector<std::string> linevect;
                boost::split(linevect, linestr, boost::is_any_of(" "));
                split_line(sys, linevect, line_num);
            }

            line_num++;
            
        }
        
        myfile.close();
    } else
        throw std::invalid_argument("Unable to open MSH file.");

    create_fil_corners(sys);

    // Initialize all filament currents to zero.
    for (auto fil : sys->filVect) {
        fil->stroom.x = 0.0;
        fil->stroom.y = 0.0;
        fil->stroom.z = 0.0;        
    }    
    
    std::cout << "--- Success reading filament file ---" << std::endl;
}

// id layer Sx Sy Sz Lx Ly Lz Wx Wy Wz Hx Hy Hz length width height area
// 0  1     2  3  4  5  6  7  8  9  10 11 12 13 14     15    16     17
void split_line(System *sys, std::vector<std::string> linevect, int num) {
    double um = 10e12;

    Filament *fil = new Filament;

    fil->id = std::stod(linevect[0]);
    fil->layer = linevect[1];
    
    fil->S.x = std::stod(linevect[2]) * um;
    fil->S.y = std::stod(linevect[3]) * um;
    fil->S.z = std::stod(linevect[4]) * um;

    fil->L.x = std::stod(linevect[5]);
    fil->L.y = std::stod(linevect[6]);
    fil->L.z = std::stod(linevect[7]);

    fil->W.x = std::stod(linevect[8]);
    fil->W.y = std::stod(linevect[9]);
    fil->W.z = std::stod(linevect[10]);

    fil->H.x = std::stod(linevect[11]);
    fil->H.y = std::stod(linevect[12]);
    fil->H.z = std::stod(linevect[13]);
    
    fil->length = std::stod(linevect[14]) * um;
    fil->width = std::stod(linevect[15]) * um;
    fil->height = std::stod(linevect[16]) * um;
    fil->area = std::stod(linevect[17]) * um;

    sys->filVect.push_back(fil);
}

void read_mat(System *sys, std::string mat_name) {
    mat_name = mat_name + ".mat";
    std::string mat_file = sys->m_files["dir"] + mat_name;
    std::ifstream myfile(mat_file);
    std::string line;
    
    std::cout << "Reading port: " << "\033[1;35m" + mat_name + "\033[0m" << std::endl;

    int num = 0;
    
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            std::stringstream ss(line);
            std::string word;
            std::string linestr(line);

            std::vector<std::string> linevect;
            boost::trim_if(linestr, boost::is_any_of("\t "));
            boost::split(linevect, linestr, boost::is_any_of("\t "), boost::token_compress_on);

            double x = std::stod(linevect[3]);

            sys->filVect[num]->stroom.x += x;
            sys->filVect[num]->stroom.y += std::stod(linevect[4]);
            sys->filVect[num]->stroom.z += std::stod(linevect[5]);
    
            num++;
        }

        myfile.close();
    } else
        throw std::invalid_argument("Unable to open MSH file.");
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










