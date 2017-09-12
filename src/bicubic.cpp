#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include "sys.h"

using namespace std;

void calc_bicubic(System *sys, Node **color_values, double *w_x, double *w_y) {
    double row_1 = color_values[0][0].x * w_x[0] + color_values[0][1].x * w_x[1] + color_values[0][2].x * w_x[2] +
                   color_values[0][3].x * w_x[3];
    double row_2 = color_values[1][0].x * w_x[0] + color_values[1][1].x * w_x[1] + color_values[1][2].x * w_x[2] +
                   color_values[1][3].x * w_x[3];
    double row_3 = color_values[2][0].x * w_x[0] + color_values[2][1].x * w_x[1] + color_values[2][2].x * w_x[2] +
                   color_values[2][3].x * w_x[3];
    double row_4 = color_values[3][0].x * w_x[0] + color_values[3][1].x * w_x[1] + color_values[3][2].x * w_x[2] +
                   color_values[3][3].x * w_x[3];

    sys->val->x = w_y[0] * row_1 + w_y[1] * row_2 + w_y[2] * row_3 + w_y[3] * row_4;

    row_1 = color_values[0][0].y * w_x[0] + color_values[0][1].y * w_x[1] + color_values[0][2].y * w_x[2] +
            color_values[0][3].y * w_x[3];
    row_2 = color_values[1][0].y * w_x[0] + color_values[1][1].y * w_x[1] + color_values[1][2].y * w_x[2] +
            color_values[1][3].y * w_x[3];
    row_3 = color_values[2][0].y * w_x[0] + color_values[2][1].y * w_x[1] + color_values[2][2].y * w_x[2] +
            color_values[2][3].y * w_x[3];
    row_4 = color_values[3][0].y * w_x[0] + color_values[3][1].y * w_x[1] + color_values[3][2].y * w_x[2] +
            color_values[3][3].y * w_x[3];

    sys->val->y = w_y[0] * row_1 + w_y[1] * row_2 + w_y[2] * row_3 + w_y[3] * row_4;

    row_1 = color_values[0][0].z * w_x[0] + color_values[0][1].z * w_x[1] + color_values[0][2].z * w_x[2] +
            color_values[0][3].z * w_x[3];
    row_2 = color_values[1][0].z * w_x[0] + color_values[1][1].z * w_x[1] + color_values[1][2].z * w_x[2] +
            color_values[1][3].z * w_x[3];
    row_3 = color_values[2][0].z * w_x[0] + color_values[2][1].z * w_x[1] + color_values[2][2].z * w_x[2] +
            color_values[2][3].z * w_x[3];
    row_4 = color_values[3][0].z * w_x[0] + color_values[3][1].z * w_x[1] + color_values[3][2].z * w_x[2] +
            color_values[3][3].z * w_x[3];

    sys->val->z = w_y[0] * row_1 + w_y[1] * row_2 + w_y[2] * row_3 + w_y[3] * row_4;
}

void get_color(System *sys, int i, int j, Node **color_values, int n, int m) {
    double weight_x[4];
    double weight_y[4];

    double Sx = ((double) (i - sys->grid_width * n)) / sys->grid_width;
    double Sy = ((double) (j - sys->grid_width * m)) / sys->grid_width;

    weight_x[0] = (-pow(Sx, 3) + 2 * pow(Sx, 2) - Sx) / 2;
    weight_x[1] = (3 * pow(Sx, 3) - 5 * pow(Sx, 2) + 2) / 2;
    weight_x[2] = (-3 * pow(Sx, 3) + 4 * pow(Sx, 2) + Sx) / 2;
    weight_x[3] = (pow(Sx, 3) - pow(Sx, 2)) / 2;

    weight_y[0] = (-pow(Sy, 3) + 2 * pow(Sy, 2) - Sy) / 2;
    weight_y[1] = (3 * pow(Sy, 3) - 5 * pow(Sy, 2) + 2) / 2;
    weight_y[2] = (-3 * pow(Sy, 3) + 4 * pow(Sy, 2) + Sy) / 2;
    weight_y[3] = (pow(Sy, 3) - pow(Sy, 2)) / 2;

    calc_bicubic(sys, color_values, weight_x, weight_y);
}

void shift_color_array(System *sys, Node **array, int n, int m, Node **new_colors) {
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            new_colors[i][j].x = array[m + i - 1][n + j - 1].x;
            new_colors[i][j].y = array[m + i - 1][n + j - 1].y;
            new_colors[i][j].z = array[m + i - 1][n + j - 1].z;
        }
    }
}

void bicubic(System *sys) {
    printf("Start Bicubic interpolation\n");

    int i, j, n, m;

    Node **color_array = (Node **) calloc(4, sizeof(Node *));

    for (n = 0; n < 4; n++)
        color_array[n] = (Node *) calloc(4, sizeof(Node));

    sys->val = (Node *) calloc(4, sizeof(Node));

    for (i = 0; i < sys->NGrid; i++) {
        for (j = 0; j < sys->NGrid; j++) {
            for (n = 1; n < sys->factor - 2; n++) {
                for (m = 1; m < sys->factor - 2; m++) {
                    if (i >= sys->grid_width * n && i < (sys->grid_width * n + sys->grid_width)) {
                        if (j >= sys->grid_width * m && j < (sys->grid_width * m + sys->grid_width)) {
                            shift_color_array(sys, sys->BMag, n, m, color_array);
                            get_color(sys, i, j, color_array, n, m);

                            sys->BField[i][j].x = sys->val->x;
                            sys->BField[i][j].y = sys->val->y;
                            sys->BField[i][j].z = sys->val->z;
                        }
                    }
                }
            }
        }
    }

    printf("Bicubic interpolation finished...\n");
}





























