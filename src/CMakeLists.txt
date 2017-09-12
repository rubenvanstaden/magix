#Set executable
set(MAGIX_SRC 
# ${CMAKE_CURRENT_SOURCE_DIR}/bicubic.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/biot_savart.cpp
#               ${CMAKE_CURRENT_SOURCE_DIR}/boolean_poly_operation.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/clipper.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/geometry.cpp
#               ${CMAKE_CURRENT_SOURCE_DIR}/get_all_examples.cpp
#               ${CMAKE_CURRENT_SOURCE_DIR}/grid.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/images.cpp
#               ${CMAKE_CURRENT_SOURCE_DIR}/interpolate_filaments.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/lic.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#               ${CMAKE_CURRENT_SOURCE_DIR}/mergeSort.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/read.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/polygons.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/tools.cpp              
              # ${CMAKE_CURRENT_SOURCE_DIR}/vtk_field.cpp
            #   ${CMAKE_CURRENT_SOURCE_DIR}/vtk_layers.cpp
              ${CMAKE_CURRENT_SOURCE_DIR}/vtk_grid.cpp
              # ${CMAKE_CURRENT_SOURCE_DIR}/vtk_structure.cpp              
              PARENT_SCOPE)
