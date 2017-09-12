#define BOOST_FILESYSTEM_VERSION 3

#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#  define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
#ifndef BOOST_SYSTEM_NO_DEPRECATED
#  define BOOST_SYSTEM_NO_DEPRECATED
#endif

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>

namespace fs = boost::filesystem;

void get_all_examples(std::string testString) {

    std::string origin("def_tests/");
    std::string test("def_tests/" + testString);
    fs::path p(test);

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(p)) {
        printf("\n%c[1;31mERROR:\033[0m ", 27);
        printf("Example file specified does not exist\n");
        printf("Use ");
        printf("%c[1;36m-s\033[0m", 27);
        printf(" or ");
        printf("%c[1;36m--samples\033[0m", 27);
        printf(" to view usable examples\n\n");

        exit(0);
    }

    if (fs::is_directory(p)) {
        std::cout << "\nIn directory: " << p << "\n\n";
        fs::directory_iterator end_iter;

        for (fs::directory_iterator dir_itr(p); dir_itr != end_iter; ++dir_itr) {
            try {
                if (fs::is_directory(dir_itr->status())) {
                    ++dir_count;
                    std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if (fs::is_regular_file(dir_itr->status())) {
                    ++file_count;
                    std::cout << dir_itr->path().filename() << "\n";
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

        std::cout << "\n" << file_count << " files\n"
        << dir_count << " directories\n"
        << other_count << " others\n"
        << err_count << " errors\n";
    }
    else
        std::cout << "\nFound: " << p << "\n";

    if (test.compare(origin) == 0)
        exit(0);
}
