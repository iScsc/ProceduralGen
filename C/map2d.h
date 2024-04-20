#ifndef MAP2D
#define MAP2D

#include "gradientGrid.h"

// ----- Structure definition -----

struct map2d
{
    int width;
    int height;
    int sizeFactor;
    double* values;
};

typedef struct map2d map2d;

// ----- Functions -----

double smoothstep(double w);

double interpolate(double a0, double a1, double w);



double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradGrid);

double perlin(double x, double y, gradientGrid* gradGrid);



map2d* newMap2d(gradientGrid* gradGrid, int sizeFactor);

double* getValue(map2d* map2d, int width_idx, int height_idx);


void printMap2d(map2d* map2d);

void freeMap2d(map2d* map2d);

#endif
