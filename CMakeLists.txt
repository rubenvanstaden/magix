cmake_minimum_required(VERSION 3.3)
project(magix)

find_package(VTK REQUIRED)
find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include(${VTK_USE_FILE})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Ensure C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles)

add_subdirectory(src)
                    
find_package(Armadillo REQUIRED)
find_package(VTK REQUIRED)

# Make sure all other dependencies are available
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
link_directories(${CONAN_LIB_DIRS})
include_directories(${CONAN_INCLUDE_DIRS})

include_directories(${ARMADILLO_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/local)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(magix ${MAGIX_SRC})

target_link_libraries(magix ${ARMADILLO_LIBRARIES})
target_link_libraries(magix ${VTK_LIBRARIES})
target_link_libraries(magix ${Boost_LIBRARIES})
target_link_libraries(magix ${CONAN_LIBS_DOCOPT.CPP})