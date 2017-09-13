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
#include <unistd.h>


using namespace ClipperLib;

namespace fs = boost::filesystem;

double get_dec_place(double value, int decPlace)
{
    int dec = 1;
    for (int i = 0; i < decPlace; i++)
        dec *= 10;
    return floor(value * dec + 0.5) / dec;
}

void set_example_vtk_path(System *sys, std::string test_run)
{
    sys->vtkpath = test_run + "vtk/";
    fs::path vtkfolder(sys->vtkpath);
    
    std::cout << "VTK example path: " << sys->vtkpath << std::endl;  
    if (boost::filesystem::create_directory(vtkfolder))
        std::cout << "VTK folder created" << "\n";
}

void file_system(System *sys, std::string test_name, std::vector<std::string> ports)
{
    chdir("../");

    boost::filesystem::path project_root_path(boost::filesystem::current_path());
    std::cout << "\n--- Project root path is : " << project_root_path << std::endl;

    std::string test_dir(project_root_path.string() + "/tests/");
    std::string test_run(test_dir + test_name + "/");
    fs::path test_path(test_run);
    std::cout << "Test path: " << test_path << std::endl;

    set_example_vtk_path(sys, test_run);

    for (auto const& port : ports) {
        fs::path port_path(test_run + port + ".mat");
        if (!fs::exists(port_path))
            throw std::invalid_argument("Current file does not exist. Run InductEx and FFH to generate the current files.");
    }

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
        std::cout << "Files found:" << std::endl;
        fs::directory_iterator end_iter;

        for (fs::directory_iterator dir_itr(test_path); dir_itr != end_iter; ++dir_itr) {
            try {
                if (fs::is_directory(dir_itr->status())) {
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if (fs::is_regular_file(dir_itr->status())) {
                    ++file_count;
                    std::string filename = dir_itr->path().filename().string();
                    std::string extension = fs::extension(filename);

                    // does_portfile_exist(filename, ports);

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

    // TODO: We have to change this is future versions.
    bool fil = false;
    bool mat = false;

    for (const auto& pair : sys->m_files) {
        if (pair.first == ".fil") {
            fil = true;
            std::cout << "-- " << pair.first << ": " << pair.second << "\n";            
        }
        if (pair.first == ".mat") {
            mat = true;
            std::cout << "-- " << pair.first << ": " << pair.second << "\n";
        }
    }

    if (!fil)
        throw std::invalid_argument(".fil file missing");
    if (!mat)
        throw std::invalid_argument(".mat file missing");
}