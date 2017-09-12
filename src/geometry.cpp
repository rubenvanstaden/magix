#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include "boost/progress.hpp"
#include <iostream>
#include <stdexcept>

#include "clipper.hpp"
#include "sys.h"

#ifdef _WIN32
#include <direct.h>
#elif defined _WIN64
#include <direct.h>
#elif defined __unix__
#include <unistd.h>
#elif defined __APPLE__
#include <unistd.h>
#else
printf("Unsupported operating system\n");
#endif

using namespace ClipperLib;
using namespace std;

namespace fs = boost::filesystem;


// int point_in_box(Node *p, Node *x_box, Node *y_box) {
//     if (p->x > x_box->x && p->x < x_box->y) if (p->y > y_box->x && p->y < y_box->y)
//         return 1;

//     return 0;
// }

// void sort(Paths &p, Node *box, int n, int i, int vertices) {
//     int j;

//     box->x = DBL_MAX;
//     box->y = -DBL_MAX;

//     for (j = 0; j < vertices; j++) {
//         if (n == 0) {
//             if (p[i][j].X <= box->x)
//                 box->x = p[i][j].X;
//             if (p[i][j].X >= box->y)
//                 box->y = p[i][j].X;
//         } else {
//             if (p[i][j].Y <= box->x)
//                 box->x = p[i][j].Y;
//             if (p[i][j].Y >= box->y)
//                 box->y = p[i][j].Y;
//         }
//     }

//     box->x = box->x / 10e12;
//     box->y = box->y / 10e12;
// }

double get_dec_place(double value, int decPlace) {
    int i;
    int dec = 1;

    for (i = 0; i < decPlace; i++)
        dec *= 10;

    return floor(value * dec + 0.5) / dec;
}

void file_system(System *sys, std::string test_name) {
    chdir("../");

    boost::filesystem::path project_root_path(boost::filesystem::current_path());
    std::cout << "\n--- Project root path is : " << project_root_path << std::endl;

    std::string test_dir(project_root_path.string() + "/tests/");
    std::string test_run(test_dir + test_name + "/");
    fs::path test_path(test_run);
    cout << "Test path: " << test_path << endl;

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(test_path)) {
        printf("\n%c[1;31mERROR:\033[0m ", 27);
        printf("Example file specified does not exist\n");
        printf("Use ");
        printf("%c[1;36m-s\033[0m", 27);
        printf(" or ");
        printf("%c[1;36m--samples\033[0m", 27);
        printf(" to view usable examples\n\n");

        exit(0);
    }

    sys->m_files["dir"] = test_run;

    if (fs::is_directory(test_path)) {
        std::cout << "Files found:" << endl;
        fs::directory_iterator end_iter;

        for (fs::directory_iterator dir_itr(test_path); dir_itr != end_iter; ++dir_itr) {
            try {
                if (fs::is_directory(dir_itr->status())) {
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if (fs::is_regular_file(dir_itr->status())) {
                    ++file_count;
                    string filename = dir_itr->path().filename().string();
                    string extension = fs::extension(filename);
                    sys->m_files[extension] = filename;
                }
                else {
                    ++other_count;
                    std::cout << dir_itr->path().filename() << " [other]\n";
                }
            }
            catch (const std::exception &ex) {
                ++err_count;
                std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
    }
    else
        std::cout << "\nFound: " << test_path << "\n";

    bool fil = false;
    bool mat = false;

    for (const auto& pair : sys->m_files) {
        if (pair.first == ".fil")
            fil = true;
        if (pair.first == ".mat")
            mat = true;

        std::cout << "-- " << pair.first << ": " << pair.second << "\n";
    }

    if (!fil)
        throw std::invalid_argument(".fil file missing");
    if (!mat)
        throw std::invalid_argument(".mat file missing");
}





// void set_default_options(System *sys) {
//     int i;

//     char cwd[1024];

//     printf("\n%c[1;32mStarting MagnetEx...\033[0m\n", 27);

// #ifdef _WIN32
//     if (_getcwd(cwd, sizeof(cwd)) != NULL)
//         fprintf(stdout, "Current working dir: %s\n", cwd);
//     else
//         perror("getcwd() error");
// #elif defined _WIN64
//     if (_getcwd(cwd, sizeof(cwd)) != NULL)
//         fprintf(stdout, "Current working dir: %s\n", cwd);
//     else
//         perror("getcwd() error");
// #elif defined __unix__
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//         fprintf(stdout, "Current working dir: %s\n", cwd);
//     else
//         perror("getcwd() error");
// #elif defined __APPLE__
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//         fprintf(stdout, "Current working dir: %s\n", cwd);
//     else
//         perror("getcwd() error");
// #else
//     printf("Unsupported operating system\n");
// #endif

//     sys->grid_width = 10;
//     sys->factor = ((sys->NGrid - 1) / (sys->grid_width)) + 1;

//     sys->dir = (char *) malloc(1 + strlen(cwd) + strlen("/def_tests/"));
//     strcpy(sys->dir, cwd);
//     strcat(sys->dir, "/def_tests/");
//     sys->fil_name = "/filaments.txt";

//     // sys->BMag = (Node **) calloc(sys->factor, sizeof(Node *));
//     // sys->BField = (Node **) calloc(sys->NGrid, sizeof(Node *));

//     // for (i = 0; i < sys->factor; i++)
//     //     sys->BMag[i] = (Node *) calloc(sys->factor, sizeof(Node));
//     // for (i = 0; i < sys->NGrid; i++)
//     //     sys->BField[i] = (Node *) calloc(sys->NGrid, sizeof(Node));

//     // sys->pNoise = (double *) malloc(sizeof(double) * sys->NGrid * sys->NGrid);

//     sys->mat_file = (char *) malloc(1 + strlen(sys->dir) + strlen(sys->test_name) + strlen("/") + strlen(sys->mat_name) + strlen(".mat"));
//     strcpy(sys->mat_file, sys->dir);
//     strcat(sys->mat_file, sys->test_name);
//     strcat(sys->mat_file, "/");
//     strcat(sys->mat_file, sys->mat_name);
//     strcat(sys->mat_file, ".mat");

//     printf("Directory of %c[1;31m.mat\033[0m: %s\n", 27, sys->mat_file);

//     sys->gds_file = (char *) malloc(1 + strlen(sys->dir) + strlen(sys->test_name) + strlen("/") + strlen(sys->gds_name));
//     strcpy(sys->gds_file, sys->dir);
//     strcat(sys->gds_file, sys->test_name);
//     strcat(sys->gds_file, "/");
//     strcat(sys->gds_file, sys->gds_name);
//     strcat(sys->gds_file, ".gds");

//     printf("Directory of %c[1;31m.gds\033[0m: %s\n", 27, sys->gds_file);

//     sys->fil_file = (char *) malloc(1 + strlen(sys->dir) + strlen(sys->test_name) + strlen(sys->fil_name));
//     strcpy(sys->fil_file, sys->dir);
//     strcat(sys->fil_file, sys->test_name);
//     strcat(sys->fil_file, sys->fil_name);

//     printf("Directory of %c[1;31m.fil\033[0m: %s\n", 27, sys->fil_file);
// }
