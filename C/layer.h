#ifndef LAYER
#define LAYER

#include "gradientGrid.h"

// ----- Structure definition -----

struct layer
{
    int width;
    int height;
    int size_factor;

    gradientGrid* gradient_grid;

    double* values;
};

typedef struct layer layer;

// ----- Functions -----

double smoothstep(double w);

double interpolate(double a0, double a1, double w);



double dotGridGradient(int ix, int iy, double x, double y, gradientGrid* gradient_grid);

double perlin(double x, double y, gradientGrid* gradient_grid);



double* getLayerValue(layer* layer, int width_idx, int height_idx);



layer* newLayerFromGradient(gradientGrid* gradient_grid, int size_factor, unsigned int display_loading);

layer* newLayer(int gradGrid_width, int gradGrid_height, int size_factor, unsigned int display_loading);


void printLayer(layer* layer);

void freeLayer(layer* layer);

#endif
