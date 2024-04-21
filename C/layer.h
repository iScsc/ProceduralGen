#ifndef LAYER
#define LAYER

#include "gradientGrid.h"

// ----- Structure definition -----

struct layer
{
    int width;
    int height;
    int sizeFactor;

    gradientGrid* gradGrid;

    double* values;
};

typedef struct layer layer;

// ----- Functions -----

double smoothstep(double w);

double interpolate(double a0, double a1, double w);



double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradGrid);

double perlin(double x, double y, gradientGrid* gradGrid);



layer* newLayer(gradientGrid* gradGrid, int sizeFactor, int display_loading);

double* getValue(layer* layer, int width_idx, int height_idx);


void printLayer(layer* layer);

void freeLayer(layer* layer);

#endif
