#include <math.h>
#include <iostream>

#include "sys.h"
#include "grid.h"
#include "globals.h"
#include "tools.h"

void k_from_cude(double x, double y, double z, double *delta_array) {
    double epsi = sqrt(x * x + y * y + z * z);

    delta_array[0] = (y - x * atan(y / x) + x * atan((y * z) / (x * epsi)) - z * log(2 * (y + epsi)) -
                      y * log(2 * (z + epsi)));
    delta_array[1] = (z - y * atan(z / y) + y * atan((z * x) / (y * epsi)) - x * log(2 * (z + epsi)) -
                      z * log(2 * (x + epsi)));
    delta_array[2] = (x - z * atan(x / z) + z * atan((x * y) / (z * epsi)) - y * log(2 * (x + epsi)) -
                      x * log(2 * (y + epsi)));
}

void mag_fields(System *sys, double x, double y, double z, double *K, Filament *fil) {
    double back_tr[3], front_tr[3];
    double back_tl[3], front_tl[3];
    double back_br[3], front_br[3];
    double back_bl[3], front_bl[3];

    double w = UM * fil->width / 2.0;
    double h = UM * fil->height / 2.0;
    double l = UM * fil->length;

    k_from_cude(x + w, y + h, z + l, back_tr);
    k_from_cude(x - w, y + h, z + l, back_tl);
    k_from_cude(x + w, y - h, z + l, front_tr);
    k_from_cude(x - w, y - h, z + l, front_tl);

    k_from_cude(x + w, y + h, z - l, back_br);
    k_from_cude(x - w, y + h, z - l, back_bl);
    k_from_cude(x + w, y - h, z - l, front_br);
    k_from_cude(x - w, y - h, z - l, front_bl);

    K[0] = back_tr[0] - back_tl[0] - front_tr[0] + front_tl[0] - back_br[0] + back_bl[0] + front_br[0] - front_bl[0];
    K[1] = back_tr[1] - back_tl[1] - front_tr[1] + front_tl[1] - back_br[1] + back_bl[1] + front_br[1] - front_bl[1];
    K[2] = back_tr[2] - back_tl[2] - front_tr[2] + front_tl[2] - back_br[2] + back_bl[2] + front_br[2] - front_bl[2];
}

void biot_savart(System *sys, Grid *grid) 
{
    grid->Hx.reshape(sys->NGrid, sys->NGrid);
    grid->Hy.reshape(sys->NGrid, sys->NGrid);
    grid->Hz.reshape(sys->NGrid, sys->NGrid);

    for (int i = 0; i < sys->NGrid; i++) {
        load_bar(i, sys->factor - 1, 100, 60);
        
        for (int j = 0; j < sys->NGrid; j++) {
            double K[3];
            
            double Bx = 0;
            double By = 0;
            double Bz = 0;
            
            for (auto fil : sys->filVect) {                
                double s_x = (grid->nx(i,j) - fil->S.x) * UM;
                double s_y = (grid->ny(i,j) - fil->S.y) * UM;
                double s_z = (grid->nz(i,j) - fil->S.z) * UM;
    
                mag_fields(sys, s_x, s_y, s_z, K, fil);
    
                Bx = Bx + C * (fil->stroom.y * K[2] - fil->stroom.z * K[1]);
                By = By + C * (fil->stroom.z * K[0] - fil->stroom.x * K[2]);
                Bz = Bz + C * (fil->stroom.x * K[1] - fil->stroom.y * K[0]);
            }
    
            grid->Hx(i,j) += 6 * Bx;
            grid->Hy(i,j) += 6 * By;
            grid->Hz(i,j) += 6 * Bz;
        }
    }
}