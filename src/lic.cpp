#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "sys.h"
#include <time.h>
#include "grid.h"
#include <armadillo>

using namespace std;

double compute_convolution_normal(System *sys, double *pNoise, Node *C) {
    double sum = 0;

    for (int i = 1; i < 2 * sys->len; i++) {
        int samp_y = (C[i].x + C[i - 1].x) * 0.5;
        int samp_x = (C[i].y + C[i - 1].y) * 0.5;

        if (samp_x < 0)
            samp_x = 0;
        if (samp_y < 0)
            samp_y = 0;

        if (samp_x > sys->NGrid)
            samp_x = sys->NGrid - 1;
        if (samp_y > sys->NGrid)
            samp_y = sys->NGrid - 1;

        int pic_loc = samp_y * sys->NGrid + samp_x;

        sum = sum + pNoise[pic_loc];
    }

    sum = sum / (2 * sys->len + 1);

    return sum;
}

double compute_convolution_high(System *sys, double *pNoise, Node *C) {
    double sum = 0;

    double filter[3][3] = {
        0, 1 / 2, 0,
        1 / 2, 1.5, 1 / 2,
        0, 1 / 2, 0
    };

    for (int i = 1; i < 2 * sys->len; i++) {
        int samp_x = (C[i].x + C[i - 1].x) * 0.5;
        int samp_y = (C[i].y + C[i - 1].y) * 0.5;

        if (samp_x < 0)
            samp_x = 0;
        if (samp_y < 0)
            samp_y = 0;

        if (samp_x > sys->NGrid)
            samp_x = sys->NGrid - 1;
        if (samp_y > sys->NGrid)
            samp_y = sys->NGrid - 1;

        for (int r = 0; r < 3; r++) {
            for (int s = 0; s < 3; s++) {
                int n = (samp_x - 3 / 2 + r + sys->NGrid) % sys->NGrid;
                int m = (samp_y - 3 / 2 + s + sys->NGrid) % sys->NGrid;
                int pic_loc = n * sys->NGrid + m;
                sum = sum + (pNoise[pic_loc] * filter[r][s]);
            }
        }
    }

    sum = sum / (2 * sys->len + 1);

    return sum;
}

void synthesize_vectors(System *sys, Grid *grid, double **vect_x, double **vect_y) {
    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            vect_x[j][i] = grid->Hy(j,i);
            vect_y[j][i] = grid->Hz(j,i);
        }
    }
}

void normalize_vectors(System *sys, double **vect_x, double **vect_y) {
    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            double mag = sqrtf(vect_x[j][i] * vect_x[j][i] + vect_y[j][i] * vect_y[j][i]);

            if (mag > 0.0) {
                vect_x[j][i] = vect_x[j][i] / mag;
                vect_y[j][i] = vect_y[j][i] / mag;
            }
            else {
                vect_x[j][i] = 0;
                vect_y[j][i] = 0;
            }
        }
    }
}

void white_noise(System *sys, double *pNoise) {
    int pic_loc;

    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            int r = rand();
            pic_loc = j * sys->NGrid + i;

            r = ((r & 255) + ((r & 255) >> 8)) & 255;
            pNoise[pic_loc] = (unsigned char) r;
        }
    }
}

void compute_integral_curve(System *sys, Node *p, Node *C, double **vect_x, double **vect_y) {
    double x = p->x + 0.5;
    double y = p->y + 0.5;

    int s;
    int index = 0;

    C[index].x = x;
    C[index].y = y;

    double segLen = 0;
    double vctr_x = 0;
    double vctr_y = 0;

    // positive calculations
    for (s = 0; s < sys->len; s++) {
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (x > sys->NGrid)
            x = sys->NGrid - 1;
        if (y > sys->NGrid)
            y = sys->NGrid - 1;

        vctr_x = vect_x[(int) x][(int) y];
        vctr_y = vect_y[(int) x][(int) y];

        segLen += 0.1;

        x = x + segLen * vctr_x;
        y = y + segLen * vctr_y;

        C[index].x = x;
        C[index].y = y;

        index++;
    }

    x = p->x + 0.5;
    y = p->y + 0.5;

    segLen = 0;

    // negative calculations
    for (s = 0; s < sys->len; s++) {
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (x > sys->NGrid)
            x = sys->NGrid - 1;
        if (y > sys->NGrid)
            y = sys->NGrid - 1;

        vctr_x = vect_x[(int) x][(int) y];
        vctr_y = vect_y[(int) x][(int) y];

        segLen += 0.1;

        x = x - segLen * vctr_x;
        y = y - segLen * vctr_y;

        C[index].x = x;
        C[index].y = y;

        index++;
    }
}

void lic(System *sys, Grid *grid) {
    grid->ix.reshape(sys->NGrid, sys->NGrid);
    grid->iy.reshape(sys->NGrid, sys->NGrid);
    grid->iz.reshape(sys->NGrid, sys->NGrid);

    sys->len = 20;

    std::cout << "\n--- LIC algorithm started ---" << std::endl;

    clock_t begin, end;
    double time_spent;

    begin = clock();

    Node *p = (Node *) calloc(1, sizeof(Node));
    Node *C = (Node *) calloc(2 * sys->len, sizeof(Node));

    double *pNoise = (double *) malloc(sizeof(double) * sys->NGrid * sys->NGrid);
    double *pNoise_double = (double *) malloc(sizeof(double) * sys->NGrid * sys->NGrid);

    double **vect_x = (double **) calloc(sys->NGrid, sizeof(double *));
    double **vect_y = (double **) calloc(sys->NGrid, sizeof(double *));

    for (int i = 0; i < sys->NGrid; i++)
        vect_x[i] = (double *) calloc(sys->NGrid, sizeof(double));
    for (int i = 0; i < sys->NGrid; i++)
        vect_y[i] = (double *) calloc(sys->NGrid, sizeof(double));

    synthesize_vectors(sys, grid, vect_x, vect_y);
    normalize_vectors(sys, vect_x, vect_y);
    white_noise(sys, pNoise);

    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            p->x = j;
            p->y = i;

            compute_integral_curve(sys, p, C, vect_x, vect_y);
            double sum = compute_convolution_normal(sys, pNoise, C);

            if (sum < 100)
                sum = 0;
            if (sum > 150)
                sum = 255;

            pNoise_double[j * sys->NGrid + i] = sum;
        }
    }

    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            p->x = j;
            p->y = i;

            compute_integral_curve(sys, p, C, vect_x, vect_y);
            double sum = compute_convolution_high(sys, pNoise_double, C);

            if (sum < 0)
                sum = 0;
            if (sum > 255)
                sum = 255;

            grid->ix(j,i) = sum;
        }
    }

    arma::mat magVect(sys->NGrid, sys->NGrid);

    for (int j = 0; j < sys->NGrid; j++) {
        for (int i = 0; i < sys->NGrid; i++) {
            double mag_x = grid->Hx(j,i) * grid->Hx(j,i);
            double mag_y = grid->Hy(j,i) * grid->Hy(j,i);
            double mag_z = grid->Hz(j,i) * grid->Hz(j,i);
            
            double mag = sqrtf(mag_x + mag_y + mag_z);

            if (mag > 0)
                magVect(j,i) = mag;
        }
    }

    for (int j = 0; j < sys->NGrid; j++)
        for (int i = 0; i < sys->NGrid; i++)
            grid->iy(j,i) = magVect(j,i);

    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("LIC End\n");
    printf("Time Spend: %lf\n", time_spent);
}
