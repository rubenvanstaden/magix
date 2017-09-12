#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <iostream>

#include <armadillo>
#include <tuple>

#include "sys.h"
#include "grid.h"
#include "vtk_functions.h"

using namespace std;

typedef struct {
    double minMag;
    double maxMag;
    double range;

} SYS_Image;

// void draw_image(System *sys, Grid *grid) {
//     std::cout << "Started drawing image" << endl;

//     int w = sys->NGrid;
//     int h = sys->NGrid;

//     FILE *f;
//     unsigned char *img = NULL;
//     int filesize = 54 + 3 * w * h;  
//     //w is your image width, h is image height, both int

//     img = (unsigned char *) malloc(3 * w * h);
//     memset(img, 0, sizeof(&img));

//     for (int i = 0; i < sys->NGrid; i++) {
//         for (int j = 0; j < sys->NGrid; j++) {
//             int x = i;
//             int y = j;

//             double r = sys->theImage[i][j].x;
//             double g = sys->theImage[i][j].y;
//             double b = sys->theImage[i][j].z;

//             if (r > 255) r = 255;
//             if (g > 255) g = 255;
//             if (b > 255) b = 255;

//             img[(x + y * w) * 3 + 2] = (unsigned char) (r);
//             img[(x + y * w) * 3 + 1] = (unsigned char) (g);
//             img[(x + y * w) * 3 + 0] = (unsigned char) (b);
//         }
//     }

//     unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
//     unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
//     unsigned char bmppad[3] = {0, 0, 0};

//     bmpfileheader[2] = (unsigned char) (filesize);
//     bmpfileheader[3] = (unsigned char) (filesize >> 8);
//     bmpfileheader[4] = (unsigned char) (filesize >> 16);
//     bmpfileheader[5] = (unsigned char) (filesize >> 24);

//     bmpinfoheader[4] = (unsigned char) (w);
//     bmpinfoheader[5] = (unsigned char) (w >> 8);
//     bmpinfoheader[6] = (unsigned char) (w >> 16);
//     bmpinfoheader[7] = (unsigned char) (w >> 24);
//     bmpinfoheader[8] = (unsigned char) (h);
//     bmpinfoheader[9] = (unsigned char) (h >> 8);
//     bmpinfoheader[10] = (unsigned char) (h >> 16);
//     bmpinfoheader[11] = (unsigned char) (h >> 24);

//     f = fopen("magix.bmp", "wb");

//     fwrite(bmpfileheader, 1, 14, f);
//     fwrite(bmpinfoheader, 1, 40, f);

//     for (int i = 0; i < h; i++) {
//         fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
//         fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
//     }

//     fclose(f);

//     std::cout << "Image drawn" << endl;
// }

// void set_min_max(System *sys, Grid *grid) {
//     int i, j;

//     // sys->theImage = (Node **) calloc(sys->NGrid, sizeof(Node *));

//     // for (i = 0; i < sys->NGrid; i++)
//     //     sys->theImage[i] = (Node *) calloc(sys->NGrid, sizeof(Node));

//     double *magSorted = (double *) malloc(sizeof(double) * sys->NGrid * sys->NGrid);

//     for (j = 0; j < sys->NGrid; j++) {
//         for (i = 0; i < sys->NGrid; i++) {
//             double mag = sqrtf(sys->BField[j][i].x * sys->BField[j][i].x + sys->BField[j][i].y * sys->BField[j][i].y + sys->BField[j][i].z * sys->BField[j][i].z);

//             if (mag > 0)
//                 magSorted[j * sys->NGrid + i] = log(mag);
//         }
//     }

//     mergesort(magSorted, sys->NGrid * sys->NGrid);

//     sys_image->minMag = DBL_MAX;
//     sys_image->maxMag = -DBL_MAX;

//     for (j = 0; j < sys->NGrid; j++) {
//         for (i = 0; i < sys->NGrid; i++) {
//             if (magSorted[j * sys->NGrid + i] != DBL_MAX) {
//                 if (sys_image->minMag > magSorted[j * sys->NGrid + i])
//                     sys_image->minMag = magSorted[j * sys->NGrid + i];
//                 if (sys_image->maxMag < magSorted[j * sys->NGrid + i])
//                     sys_image->maxMag = magSorted[j * sys->NGrid + i];
//             }
//         }
//     }

//     sys_image->range = sys_image->maxMag - sys_image->minMag;

//     if (sys->debug == 1) {
//       printf("\nminMag: %E\n", sys_image->minMag);
//       printf("maxMag: %E\n", sys_image->maxMag);
//       printf("minMag non log: %E\n", exp(sys_image->minMag));
//       printf("maxMag non log: %E\n", exp(sys_image->maxMag));
//     }
// }

void write_bmp(System *sys, Grid *grid) {
    double red = 0;
    double green = 0;
    double blue = 0;

    Node *x_box = (Node *) calloc(1, sizeof(Node));
    Node *y_box = (Node *) calloc(1, sizeof(Node));
    Node *point = (Node *) calloc(1, sizeof(Node));

    arma::field<arma::rowvec> image(sys->NGrid, sys->NGrid);
    arma::rowvec pixel(3);

    tuple <double, double, double> geek;

    for (int i = 0; i < sys->NGrid; i++) {
        for (int j = 0; j < sys->NGrid; j++) {
            double lic_val = grid->Hx(j,i) / 255;
            double val = 0;

            // if (sys->pImage[j][i].y > 0)
            //     val = ((get_dec_place(sys->pImage[j][i].y, 20)) - sys_image->minMag) / sys_image->range;

            if (val < 0.25) {
                red = 0;
                green = 255 * (val / 0.25);
                blue = 255;
            } else if ((val >= 0.25) && (val < 0.5)) {
                red = 0;
                green = 255;
                blue = 255 - 255 * (val - 0.25) / 0.25;
            } else if ((val >= 0.5) && (val < 0.75)) {
                red = 255 * (val - 0.5) / 0.25;
                green = 255;
                blue = 0;
            } else if ((val >= 0.75) && (val < 1.00)) {
                red = 255;
                green = 255 - 255 * (val - 0.75) / 0.25;
                blue = 0;
            } else {
                red = 255;
                green = 255 - 255 * (val - 0.75) / 0.25;
                blue = 0;
            }

            pixel(0) = red * grid->Hy(j,i) * lic_val;
            pixel(1) = green * grid->Hy(j,i) * lic_val;
            pixel(2) = blue * grid->Hy(j,i) * lic_val;

            image(i, j) = pixel;

            // if (sys->set_vtk == 1) {
            //     pixel(0) = red * grid->Hy(j,i) * lic_val;
            //     pixel(1) = green * grid->Hy(j,i) * lic_val;
            //     pixel(2) = blue * grid->Hy(j,i) * lic_val;

            //     image(i, j) = pixel;
            // } else {
            //     pixel(0) = red * lic_val;
            //     pixel(1) = green * lic_val;
            //     pixel(2) = blue * lic_val;

            //     image(i, j) = pixel;
            // }

            // if (sys->set_vtk == 1) {
            //     // sys->theImage[i][sys->NGrid - j].x = red * sys->pImage[j][i].y * lic_val;
            //     // sys->theImage[i][sys->NGrid - j].y = green * sys->pImage[j][i].y * lic_val;
            //     // sys->theImage[i][sys->NGrid - j].z = blue * sys->pImage[j][i].y * lic_val;
            // } else {
            //     // sys->theImages[i][sys->NGrid - j].x = red * lic_val;
            //     // sys->theImage[i][sys->NGrid - j].y = green * lic_val;
            //     // sys->theImage[i][sys->NGrid - j].z = blue * lic_val;
            // }

            // for (k = 0; k < (int) p.size(); k++) {
            //     sort(p, x_box, 0, k, (int) p[k].size());
            //     sort(p, y_box, 1, k, (int) p[k].size());

            //     point->x = obs_grid(i)(1); // y-coord
            //     point->y = obs_grid(j)(2); // z-coord

            //     if (point_in_box(point, x_box, y_box)) {
            //         sys->theImage[i][sys->NGrid - j].x = 0;
            //         sys->theImage[i][sys->NGrid - j].y = 0;
            //         sys->theImage[i][sys->NGrid - j].z = 0;
            //     }
            // }
        }
    }
    
    // image.save("image.field");    

    // if (sys->set_vtk == 1)
    //     vtk_mag_field(sys, image, obs_grid);
    // else
    //     draw_image(sys);
}
