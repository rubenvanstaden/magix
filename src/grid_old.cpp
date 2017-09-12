#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "sys.h"

typedef struct {
    double circuit_width;
    double circuit_height;
    double circuit_center_x;
    double circuit_center_y;
} Grid;

void min_max_values(System *sys, double *min_coord, double *max_coord, char const *axes) {
    *min_coord = DBL_MAX;
    *max_coord = -DBL_MAX;

    int i;

    for (i = 0; i < sys->fil_len; i++) {
        if (strcmp(sys->fil_array[i].layer, "m0") == 0) {
            double begin = get_dec_place(sys->fil_array[i].begin_node.x, 20);
            double end = get_dec_place(sys->fil_array[i].end_node.x, 20);

            if (end > begin) {
                if (fabs(begin - sys->x_slice) <= fabs(sys->fil_array[i].length / 2)) {
                    if (strcmp(axes, "horizontal") == 0) {
                        if (*min_coord > sys->fil_array[i].begin_node.y)
                            *min_coord = sys->fil_array[i].begin_node.y;
                        if (*max_coord < sys->fil_array[i].begin_node.y)
                            *max_coord = sys->fil_array[i].begin_node.y;
                    }
                    else if (strcmp(axes, "vertical") == 0) {
                        if (*min_coord > sys->fil_array[i].begin_node.z)
                            *min_coord = sys->fil_array[i].begin_node.z;
                        if (*max_coord < sys->fil_array[i].begin_node.z)
                            *max_coord = sys->fil_array[i].begin_node.z;
                    }
                }
            }
        }
    }
}

void scale_trans(System *sys, Grid *dim) {
    double y_min_hor = 0;
    double y_max_hor = 0;
    double y_min_ver = 0;
    double y_max_ver = 0;

    min_max_values(sys, &y_min_hor, &y_max_hor, "horizontal");
    min_max_values(sys, &y_min_ver, &y_max_ver, "vertical");

    dim->circuit_width = fabs(y_max_hor - y_min_hor);
    dim->circuit_height = fabs(y_max_ver - y_min_ver);

    dim->circuit_center_x = (y_min_hor + dim->circuit_width / 2.0);
    dim->circuit_center_y = (y_min_ver + dim->circuit_height / 2.0);

    sys->grid_point = dim->circuit_width / 100;

    if (sys->debug == 1) {
        printf("grid_point: %E\n", sys->grid_point);
        printf("%E %E\n", y_max_ver, y_min_ver);
        printf("%E %E\n", dim->circuit_width, dim->circuit_height);
        printf("%E %E\n", dim->circuit_center_x, dim->circuit_center_y);
    }
}

// arma::field<arma::rowvec> create_grid(System *sys) {
//     int i;

//     Grid *dim = (Grid *) calloc(1, sizeof(Grid));

//     scale_trans(sys, dim);

//     Node center;
//     double element_width;

//     if (sys->dimension == 0) {
//         double w = fabs(sys->box_max.x - sys->box_min.x);
//         double h = fabs(sys->box_max.y - sys->box_min.y);

//         if (w > h)
//             element_width = w;
//         else
//             element_width = h;

//         center.x = dim->circuit_center_x;
//         center.y = dim->circuit_center_y;
//     }

//     double w = (2*element_width * sys->zoom) / ((double) ((sys->NGrid)));
//     double wb = (2*element_width * sys->grid_width * sys->zoom) / ((double) ((sys->NGrid)));

//     sys->z_slice = sys->box_max.z;

//     if (sys->debug == 1)
//         printf("z slice value at: %E\n", sys->z_slice);

//     double x;

//     if (sys->top_view == 0)
//         x = sys->x_slice;
//     else
//         x = sys->z_slice + 1*fabs(sys->box_max.z - sys->box_min.z);

//     double y = center.x - sys->zoom * element_width;
//     double z = center.y - sys->zoom * element_width;

//     arma::field<arma::rowvec> obs_grid(sys->NGrid);
//     arma::rowvec node(3);

//     for (i = 0; i < sys->NGrid; i++) {
//         node(0) = x;
//         node(1) = y;
//         node(2) = z;

//         obs_grid(i) = node;

//         y += w;
//         z += w;
//     }

//     return obs_grid;

//     printf("Finished calculating grid points\n");
// }

// arma::field<arma::rowvec> create_bicubic_grid(System *sys) {
//     int i;

//     Grid *dim = (Grid *) calloc(1, sizeof(Grid));

//     scale_trans(sys, dim);

//     Node center;
//     double element_width;

//     if (sys->dimension == 0) {

//         double w = fabs(sys->box_max.x - sys->box_min.x);
//         double h = fabs(sys->box_max.y - sys->box_min.y);

//         if (w > h)
//             element_width = w;
//         else
//             element_width = h;

//         center.x = dim->circuit_center_x;
//         center.y = dim->circuit_center_y;
//     }

//     double w = (2*element_width * sys->zoom) / ((double) ((sys->NGrid)));
//     double wb = (2*element_width * sys->grid_width * sys->zoom) / ((double) ((sys->NGrid)));

//     sys->z_slice = sys->box_max.z;

//     if (sys->debug == 1)
//         printf("z slice value at: %E\n", sys->z_slice);

//     double x = sys->x_slice;

//     // if (sys->top_view == 0)
//     //     x = sys->x_slice;
//     // else
//     //     x = sys->z_slice + 1*fabs(sys->box_max.z - sys->box_min.z);

//     double y = center.x - sys->zoom * element_width;
//     double z = center.y - sys->zoom * element_width;

    // arma::field<arma::rowvec> obs_grid_bicubic(sys->factor);
    // arma::rowvec node(3);

//     y = center.x - sys->zoom * element_width;
//     z = center.y - sys->zoom * element_width;

//     for (i = 0; i < sys->factor; i++) {
//         node(0) = x;
//         node(1) = y;
//         node(2) = z;

//         obs_grid_bicubic(i) = node;

//         y += wb;
//         z += wb;
//     }

//     return obs_grid_bicubic;

//     printf("Finished calculating grid points\n");
// }

arma::field<arma::rowvec> create_grid(System *sys) {
    arma::field<arma::rowvec> obs_grid(sys->NGrid. sys->NGrid);
    arma::rowvec node(3);

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            
        }
    }
}
